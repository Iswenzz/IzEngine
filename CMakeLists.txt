cmake_minimum_required(VERSION 3.26)
project(SR_Client)

set(CMAKE_CXX_STANDARD 23)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # Windows-specific configuration
    message("Windows Configuration Detected!")
    set(COD4_Miles $ENV{COD4_Root}/miles)
else()
    # Non-Windows configuration
    message("Non-Windows Configuration Detected!")

    # Change this to your local 'Call of Duty 4/miles' folder!
    set(COD4_Miles
        "/run/media/nitro/fae44900-26f0-45cc-b18a-a5303f6fc202/SteamLibrary/steamapps/common/Call of Duty 4/miles"
    )

    # Cross-Compilation Setup
    set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
    set(CMAKE_FIND_ROOT_PATH /usr/bin)

    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_CROSSCOMPILING 1)
endif()

add_library(SR_Client SHARED
        main.cpp
        main.h
)

target_link_libraries(SR_Client PRIVATE
    -static-libgcc
    -static-libstdc++
    -static -lwinpthread
)

set_target_properties(SR_Client PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    PREFIX ""
    SUFFIX .dll
)

add_custom_command(TARGET SR_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/SR_Client.dll
    ${COD4_Miles}/mssSRClient.asi
)