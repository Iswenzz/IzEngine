@echo off

:: Build
echo [+] Build
rmdir build /s /q
mkdir build
cd build
cmake .. -A Win32 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=.vcpkg/windows.cmake
cmake --build . --config Release

:: Binary
copy /v src\Release\IW3SR.dll "%COD4%\miles\mssIW3SR.asi"
