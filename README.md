# neurobiomech - Stimulating the rehabilitation

`neurobiomech` is a piece of software designed to help clinicians and researche during using transcranial magnetic stimulation as well as functional electrical stimulation.

## Status
| Type | Status |
|---|---|
| License | <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/license-MIT-success" alt="License"/></a> |            |

# Table of Contents

- [neurobiomech - Stimulating the rehabilitation](#neurobiomech---stimulating-the-rehabilitation)
  - [Status](#status)
- [Table of Contents](#table-of-contents)
- [How to install](#how-to-install)
  - [Install dependencies](#install-dependencies)
  - [Compile the project](#compile-the-project)
    - [From the console](#from-the-console)
    - [Using vscode](#using-vscode)
    - [Dependencies](#dependencies)
    - [CMake](#cmake)
- [How to use](#how-to-use)
- [How to contribute](#how-to-contribute)
- [Graphical User Interface (GUI)](#graphical-user-interface-gui)
- [Documentation](#documentation)
- [Troubleshoots](#troubleshoots)
- [Cite](#cite)

# How to install

To use the `neurobiomech` backend, you must compile the source code yourself. At least one windows system is required.

## Install dependencies

Please refer to the [dependencies](#dependencies) section to know what you need to install before compiling the project.
This should install all the required dependencies to compile the project, including the tools to compile the project.

## Compile the project

### From the console

From the root of the project, create a build folder and go into it:

```bash
mkdir build
cd build
```

Then, you can compile the project using the following command:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=path/to/install -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_DOC=ON
make
make install
```

### Using vscode

If you are using vscode, you can use the `CMake Tools` extension to compile the project. You can find the `CMake Tools` extension [here](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).

Once you have installed the extension (and after having the conda environment prepared), you can copy-paste the `.vscode/settings.json.default` file to `.vscode/settings.json` and then modify the `DCMAKE_INSTALL_PREFIX` by replacing `{PATH_TO_INSTALL_DIR}` by the path to the conda environment, e.g. `"-DCMAKE_INSTALL_PREFIX=C:/Anaconda3/envs/stimalker"`.

Then, you can open the project in vscode and then press `Ctrl+Shift+P` and then type `CMake: Configure` and then `CMake: Build`. This will compile the project.

### Dependencies

`neurobiomech` relies on some other libraries that one must install prior to compiling. Fortunately, all these dependencies are also hosted on the *conda-forge* channel of Anaconda. Therefore creating (and keeping up-to-date) a conda environment should allow to install everything you need to compile

```bash
conda env create -f environment.yml  # This will create a conda environment called "neurobiomech"
conda activate neurobiomech
```

or

```bash
conda env update -f environment.yml  # This will update the conda environment called "neurobiomech"
conda activate neurobiomech
```

### Device dependencies

##### Delsys EMG

You must have a windows system to install the dependencies for the Delsys EMG.
Got to the Delsys website and download either the EMGworks software or the Trigno Software Development Kit Server. Both are served with Trigno Control Utility (TCU), which is the link between the software and the EMG box.
If TCU crashes on start or when you plug in the EMG box, please consult [troubleshooting](#Troubleshoots).

##### RehaStim2

No special setup needed, just plug in the machine using USB.

##### NIDaqmx

Linux : refer to the explanations on NI [website](https://www.ni.com/docs/en-US/bundle/ni-platform-on-linux-desktop/page/ni-platform-on-linux-desktop-definition.html) according to your distro.

### CMake

`neurobiomech` comes with a CMake (https://cmake.org/) project. If you don't know how to use CMake, you will find many examples on Internet. The main variables to set are:

> `CMAKE_INSTALL_PREFIX` Which is the `path/to/install` `neurobiomech` in. This `path/to/install` should point the base path of the environment returned by `conda env list`.
>
> `CMAKE_BUILD_TYPE` Which type of build you want. Options are `Debug`, `RelWithDebInfo`, `MinSizeRel` or `Release`. This is relevant only for the build done using the `make` command. Please note that the software will be slow if you compile it without any optimization (i.e. `Debug`).
>
> `BUILD_TESTS` If you want (`ON`) or not (`OFF`) to build the tests of the project. Please note that this will automatically download gtest (https://github.com/google/googletest). Default is `OFF`.
>
> `BUILD_DOC` If you want (`ON`) or not (`OFF`) to build the documentation of the project. Default is `OFF`.
>
> `SKIP_ASSERT` If you want (`ON`) or not (`OFF`) to skip the asserts in the functions (e.g. checks for sizes). Default is `OFF`. Putting this to `OFF` reduces the risks of Segmentation Faults, it will however slow down the code.

# How to use

TODO

# How to contribute

You are very welcome to contribute to the project! There are to main ways to contribute.

The first way is to actually code new features for `neurobiomech`. The easiest way to do so is to fork the project, make the modifications and then open a pull request to the main project. Don't forget to add your name to the contributor in the documentation of the page if you do so!

The second way is to open issues to report bugs or to ask for new features. I am trying to be as reactive as possible, so don't hesitate to do so!

# Graphical User Interface (GUI)

See the `frontend` folder

# Documentation

The documentation is automatically generated using Doxygen (http://www.doxygen.org/). You can compile it yourself if you want (by setting `BUILD_DOC` to `ON`).

# Troubleshoots

Despite our best effort to make a bug-free software, `neurobiomech` may fails sometimes. If it does, please refer to the section below to know what to do. We will fill this section with the issue over time.

#### Using Trigno Control Utility (TCU) for Delsys EMG

Requirements : Windows 7+ system

1) Install TCU normally using either the SDK or the EMGWorks installing software
2) Plug the EMG in using the USB (make sure it is powered)
3) Open the device manager
4) Open the `Universal Serial Bus controllers` category
5) Uninstall the `USBXpress` device and driver (make sure to delete all data)
6) Unplug the EMG USB, wait a few seconds then plug it back
7) After a few seconds, Windows will automatically find the right driver and install it in the background.
8) You can now use TCU.

#### NIDaqmx libs not found by CMake automatically

According to your distro, the NIDAQmx header and libs might not be in the good directory.

Known locations :
/usr/include/NIDAQmx.h
/usr/lib/x86_64-linux-gnu/libnidaqmx.so.xx.x.x (do use libnidaqmx.so as it is a symlink to the right version)

# Cite

@misc{cherniStimwalker2021,
  title = {The Stimwalker: Stimulating the rehabilitation},
  shorttitle = {Stimwalker},
  author = {Michaud, Benjamin; Hugo, Dumoulin and Cherni, Yosra},
  date = {2024-06-01},
  url = {https://github.com/LabNNL/neurobiomech_software},
  urldate = {2024-07-01},
  langid = {english}
}
```
