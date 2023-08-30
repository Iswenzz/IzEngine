@echo off
set ROOT=%cd%

:: Deps
echo [+] Dependencies
cd deps\minhook
conan create . --profile %ROOT%\.conan\windows.conf
cd %ROOT%

:: Build
echo [+] Build
mkdir build
cd build
conan install .. --build missing --profile %ROOT%\.conan\windows.conf
cmake .. -A Win32
