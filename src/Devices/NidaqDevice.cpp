#include "Devices/NidaqDevice.h"

#include "NIDAQmx.h"
#include "Devices/Generic/Exceptions.h"
#include "Devices/Data/CollectorData.h"

#include <iostream>
using namespace STIMWALKER_NAMESPACE::devices;

NidaqDevice::NidaqDevice(
    int nbChannels,
    int frameRate) : m_nbChannels(nbChannels),
                     m_frameRate(frameRate),
                     m_isConnected(false),
                     m_isRecording(false)
{
    taskHandle = 0;
}

NidaqDevice::~NidaqDevice()
{
    dispose();
}

int NidaqDevice::getNbChannels() const
{
    return m_nbChannels;
}

int NidaqDevice::getFrameRate() const
{
    return m_frameRate;
}

bool NidaqDevice::getIsConnected() const
{
    return m_isConnected;
}

void NidaqDevice::connect()
{
    if (getIsConnected())
    {
        throw DeviceIsConnectedException("The device is already connected");
    }

    connectInternal();

    m_isConnected = true;
}

inline void handleError(int err)
{
    if (err == 0)
        return;

    char error[1024];
    DAQmxGetErrorString(err, error, 1024);
    std::cout << error << std::endl;
}

void NidaqDevice::connectInternal()
{
    TaskHandle taskHandle = 0;
    int32 error = 0;
    char errBuff[2048] = {'\0'};

    error = DAQmxCreateTask("", &taskHandle);
    if (error)
        goto Error;

    error = DAQmxCreateAIVoltageChan(taskHandle, "deviceName", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL);
    if (error)
        goto Error;

    error = DAQmxStartTask(taskHandle);
    if (error)
        goto Error;

    return;

Error:
    if (taskHandle != 0)
    {
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
    }
    DAQmxGetExtendedErrorInfo(errBuff, 2048);
    throw std::runtime_error(errBuff);
}

void NidaqDevice::disconnect()
{
    if (isRecording())
    {
        throw DeviceIsRecordingException("The device is currently recording");
    }

    if (!getIsConnected())
    {
        throw DeviceIsNotConnectedException("The device is not connected");
    }

    disconnectInternal();

    m_isConnected = false;
}

void NidaqDevice::disconnectInternal()
{
    if (taskHandle != 0)
    {
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
        taskHandle = 0;
    }
}

void NidaqDevice::dispose()
{
    try
    {
        stopRecording();
    }
    catch (const DeviceIsNotRecordingException &e)
    {
        // Do nothing
    }

    try
    {
        disconnect();
    }
    catch (const DeviceIsNotConnectedException &e)
    {
        // Do nothing
    }
}

bool NidaqDevice::isRecording() const
{
    return m_isRecording;
}

void NidaqDevice::startRecording()
{
    if (!getIsConnected())
    {
        throw DeviceIsNotConnectedException("The device is not connected");
    }
    if (isRecording())
    {
        throw DeviceIsRecordingException("The device is already recording");
    }

    startRecordingInternal();

    std::lock_guard<std::mutex> lock(m_recordingMutex);
    m_isRecording = true;
}

void NidaqDevice::startRecordingInternal()
{
    int32 error = 0;

    error = DAQmxCfgSampClkTiming(taskHandle, "", 1000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000);
    if (error)
    {
        std::cerr << "Error configuring sample clock: " << error << std::endl;
        DAQmxClearTask(taskHandle);
        return;
    }

    error = DAQmxStartTask(taskHandle);
    if (error)
    {
        std::cerr << "Error starting task: " << error << std::endl;
        DAQmxClearTask(taskHandle);
        return;
    }
}

void NidaqDevice::stopRecording()
{
    if (!isRecording())
    {
        throw DeviceIsNotRecordingException("The device is not recording");
    }

    stopRecordingInternal();

    std::lock_guard<std::mutex> lock(m_recordingMutex);
    m_isRecording = false;
}

void NidaqDevice::stopRecordingInternal()
{
    if (taskHandle != 0)
    {
        int32 error = DAQmxStopTask(taskHandle);
        if (error != 0)
        {
            char errorString[2048];
            DAQmxGetErrorString(error, errorString, 1024);
            std::cout << errorString << std::endl;
        }

        DAQmxClearTask(taskHandle);
        taskHandle = 0;
    }
}

int NidaqDevice::onNewData(std::function<void(const CollectorData &newData)> callback)
{
    static int listenerId = 0;

    std::lock_guard<std::mutex> lock(m_recordingMutex);
    m_listeners[listenerId] = std::function<void(const CollectorData &)>(callback);

    return listenerId++;
}

void NidaqDevice::removeListener(int listenerId)
{
    std::lock_guard<std::mutex> lock(m_recordingMutex);
    m_listeners.erase(listenerId);
}

std::vector<CollectorData> NidaqDevice::getData() const
{
    return m_data;
}

CollectorData NidaqDevice::getData(int index) const
{
    return m_data[index];
}

void NidaqDevice::notifyListeners(const CollectorData &newData)
{
    std::lock_guard<std::mutex> lock(m_recordingMutex);
    for (const auto &listener : m_listeners)
    {
        listener.second(newData);
    }
}

// Mock implementation
NidaqDeviceMock::NidaqDeviceMock(
    int nbChannels,
    int frameRate) : NidaqDevice(nbChannels, frameRate)
{
}

void NidaqDeviceMock::startRecordingInternal()
{

    {
        std::lock_guard<std::mutex> lock(m_recordingMutex);
        m_generateData = true;
    }

    m_recordingThread = std::thread(&NidaqDeviceMock::generateData, this);
}

void NidaqDeviceMock::stopRecordingInternal()
{
    {
        std::lock_guard<std::mutex> lock(m_recordingMutex);
        m_isRecording = false;
        m_generateData = false;
    }
    if (m_recordingThread.joinable())
        m_recordingThread.join();
}

void NidaqDeviceMock::generateData()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(m_recordingMutex);
            if (!m_generateData)
                break;
        }

        // Generate data
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto data = std::vector<double>(getNbChannels(), 0.0);
        CollectorData newData(now, data);
        notifyListeners(newData);

        std::this_thread::sleep_for(std::chrono::milliseconds(1 / m_frameRate));
    }
}
