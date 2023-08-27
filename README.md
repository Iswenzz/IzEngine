# IW3SR

[![Checks](https://img.shields.io/github/checks-status/Iswenzz/IW3SR/master?logo=github)](https://github.com/Iswenzz/IW3SR/actions)
[![CodeFactor](https://img.shields.io/codefactor/grade/github/Iswenzz/IW3SR?label=codefactor&logo=codefactor)](https://www.codefactor.io/repository/github/iswenzz/IW3SR)
[![CodeCov](https://img.shields.io/codecov/c/github/Iswenzz/IW3SR?label=codecov&logo=codecov)](https://codecov.io/gh/Iswenzz/IW3SR)
[![License](https://img.shields.io/github/license/Iswenzz/IW3SR?color=blue&logo=gitbook&logoColor=white)](https://github.com/Iswenzz/IW3SR/blob/master/LICENSE)

## Building (Linux)
_Pre-Requisites:_
1. [CMake](https://cmake.org/) and [Conan](https://conan.io/).

    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install nasm:i386 build-essential gcc-multilib g++-multilib

_Build Command:_

    mkdir build && cd build
    conan install .. --build missing --profile ../.conan/linux.conf
    cmake ..
    cmake --build .

## Building (Windows)
_Pre-Requisites:_
1. [Visual Studio](https://visualstudio.microsoft.com/)
2. [CMake](https://cmake.org/) and [Conan](https://conan.io/).

_Build Command:_

    mkdir build && cd build
    conan install .. --build missing --profile ../.conan/windows.conf
    cmake .. -A Win32
    cmake --build .

### [Download](https://github.com/Iswenzz/IW3SR/releases)

## Contributors

***Note:*** If you would like to contribute to this repository, feel free to send a pull request, and I will review your code.
Also feel free to post about any problems that may arise in the issues section of the repository.

<a href="https://github.com/DavidMRyan"><img src="https://avatars.githubusercontent.com/u/39206040?v=4" height=64 style="border-radius: 50%"></a>
