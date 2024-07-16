#include <iostream>
#include <gtest/gtest.h>

#include "Devices/NidaqDevice.h"
#include "Devices/Generic/Exceptions.h"

static double requiredPrecision(1e-10);

using namespace STIMWALKER_NAMESPACE;

// Start the tests

class NidaqDeviceTest : public ::testing::Test
{
protected:
    stimwalker::devices::NidaqDevice device;

    NidaqDeviceTest() : device(0, 0)
    { // Use appropriate values for your test
        // Initialization code if needed
    }

    void SetUp() override
    {
        // Setup code if needed
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST(Nidaq, channels)
{
    auto nidaq = devices::NidaqDeviceMock(4, 1000);

    ASSERT_EQ(nidaq.getNbChannels(), 4);
    ASSERT_EQ(nidaq.getFrameRate(), 1000);

    nidaq.dispose();
}

TEST(Nidaq, connect)
{
    auto nidaq = devices::NidaqDeviceMock(4, 1000);

    ASSERT_EQ(nidaq.getIsConnected(), false);

    nidaq.connect();
    ASSERT_EQ(nidaq.getIsConnected(), true);

    EXPECT_THROW(nidaq.connect(), devices::DeviceIsConnectedException);

    nidaq.startRecording();
    EXPECT_THROW(nidaq.disconnect(), devices::DeviceIsRecordingException);
    nidaq.stopRecording();

    nidaq.disconnect();
    ASSERT_EQ(nidaq.getIsConnected(), false);

    EXPECT_THROW(nidaq.disconnect(), devices::DeviceIsNotConnectedException);

    nidaq.dispose();
}

TEST(Nidaq, recording)
{
    auto nidaq = devices::NidaqDeviceMock(4, 1000);
    ASSERT_EQ(nidaq.isRecording(), false);

    EXPECT_THROW(nidaq.startRecording(), devices::DeviceIsNotConnectedException);

    nidaq.connect();
    nidaq.startRecording();
    ASSERT_EQ(nidaq.isRecording(), true);

    EXPECT_THROW(nidaq.startRecording(), devices::DeviceIsRecordingException);

    nidaq.stopRecording();
    ASSERT_EQ(nidaq.isRecording(), false);

    EXPECT_THROW(nidaq.stopRecording(), devices::DeviceIsNotRecordingException);

    nidaq.dispose();
}

TEST(Nidaq, callback)
{
    auto nidaq = devices::NidaqDeviceMock(4, 1000);
    nidaq.connect();
    nidaq.startRecording();

    bool callbackCalled = false;
    auto callback = [&callbackCalled](const devices::CollectorData &newData)
    {
        callbackCalled = true;
    };
    nidaq.onNewData(callback);

    ASSERT_EQ(callbackCalled, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_EQ(callbackCalled, true);

    nidaq.dispose();
}

TEST_F(NidaqDeviceTest, ConnectInternal)
{
    // Simuler les conditions nécessaires avant l'appel
    device.connectInternal();
    // Vérifier l'état attendu ou le comportement après l'appel
    // ASSERT_TRUE(condition) ou EXPECT_TRUE(condition) selon le cas
}

TEST_F(NidaqDeviceTest, DisconnectInternal)
{
    // Assurez-vous que l'appareil est connecté avant de tester la déconnexion
    device.connectInternal();
    device.disconnectInternal();
    // Vérifier que la ressource est libérée ou que l'état est nettoyé
}

TEST_F(NidaqDeviceTest, StartRecordingInternal)
{
    device.connectInternal();
    device.startRecordingInternal();
    // Vérifier que l'enregistrement a démarré correctement
    // Cela peut nécessiter de vérifier l'état interne ou un indicateur spécifique
}

TEST_F(NidaqDeviceTest, StopRecordingInternal)
{
    device.connectInternal();
    device.startRecordingInternal();
    device.stopRecordingInternal();
    // Vérifier que l'enregistrement s'est arrêté correctement
    // Comme pour startRecordingInternal, cela peut nécessiter de vérifier l'état interne
}