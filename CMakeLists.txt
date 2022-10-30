cmake_minimum_required(VERSION 3.17)
project(lbnes)
set(CMAKE_CXX_STANDARD 17)

set(LBNES_VERSION 1.1)

#[===[  Changelist:

v1.1 - added menu with imgui
     - added arguments
     - fix occasional color corruption
     - eliminated need for any configuration file (though you can still use one)
     - switched to mingw64 on Windows
     - performance optimizations
     - VSync option

v1.0 - initial version

]===]#

add_compile_definitions(LBNES_VERSION="${LBNES_VERSION}")

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set, defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_C_COMPILER)
    message(STATUS "CMAKE_C_COMPILER is not set, defaulting to gcc")
    set(CMAKE_C_COMPILER gcc)
endif()

if(NOT CMAKE_CXX_COMPILER)
    message(STATUS "CMAKE_CXX_COMPILER is not set, defaulting to g++")
    set(CMAKE_CXX_COMPILER g++)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/yaml-cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui/backends)
include_directories(${PROJECT_SOURCE_DIR}/lib/portable-file-dialogs)
include_directories(${PROJECT_SOURCE_DIR}/lib/CppSockets)


set(SOURCES
    src/bus/bus.cpp
    src/devices/cpu/6502/6502.cpp
    src/devices/cpu/6502/6502_addressing_modes.cpp
    src/devices/cpu/6502/6502_instructions.cpp
    src/devices/memories/memory.cpp
    src/devices/device.cpp
    src/devices/memories/ram.cpp
    src/devices/cartridges/cartridge.cpp
    src/devices/cartridges/read_ines.cpp
    src/devices/ppu/ppu.cpp
    src/devices/memories/ppu_palette_ram.cpp
    src/devices/memories/ppu_nametable_ram.cpp
    src/devices/ppu/oam.cpp src/devices/inputs/joypad.cpp
    src/graphics/sdl_manager.cpp
    src/nes.cpp
    src/options/configurator.cpp
    src/options/arguments_manager.cpp
    src/misc/base64.cpp
    src/graphics/imgui_manager.cpp
    src/network/server.cpp
    src/network/client.cpp)


set (INCLUDES
    include/bus/bus.h
    include/devices/cpu/6502.h
    include/devices/device.h
    include/devices/memories/memory.h
    include/forwards/classes.h
    include/misc/macros.h
    include/devices/memories/ram.h
    include/devices/cartridges/cartridge.h
    include/devices/cartridges/exceptions/cartridge_exception.h
    include/devices/ppu/ppu.h
    include/devices/ppu/exceptions/ppu_exception.h
    include/devices/memories/ppu_palette_ram.h
    include/devices/memories/ppu_nametable_ram.h
    include/devices/ppu/oam.h
    include/devices/inputs/joypad.h
    include/graphics/sdl_manager.h
    include/nes.h
    include/options/configurator.hpp
    include/options/arguments_manager.hpp
    include/misc/base64.h
    include/graphics/imgui_manager.hpp
    include/network/server.hpp
    include/network/client.hpp)


set (IMGUI_SOURCES_AND_INCLUDES
    lib/imgui/imconfig.h
    lib/imgui/imgui.cpp
    lib/imgui/imgui.h
    lib/imgui/imgui_internal.h
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imstb_rectpack.h
    lib/imgui/imstb_textedit.h
    lib/imgui/imstb_truetype.h
    lib/imgui/backends/imgui_impl_sdl.h
    lib/imgui/backends/imgui_impl_sdl.cpp
    lib/imgui/backends/imgui_impl_sdlrenderer.h
    lib/imgui/backends/imgui_impl_sdlrenderer.cpp)


set (INCLUDES
    ${INCLUDES}
    lib/portable-file-dialogs/portable-file-dialogs.h)


set (INCLUDES
    ${INCLUDES}
    lib/CppSockets/SocketCompat.hpp
    lib/CppSockets/TcpServerSocket.hpp
    lib/CppSockets/TcpClientSocket.hpp
    lib/CppSockets/TcpSocket.hpp
    lib/CppSockets/UdpClientSocket.hpp
    lib/CppSockets/UdpServerSocket.hpp
    lib/CppSockets/UdpSocket.hpp)


file(GLOB IMGUI_SOURCES ${IMGUI_SOURCES_AND_INCLUDES})
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "lib/imgui")
target_include_directories(imgui PUBLIC "lib/imgui/backends")

if(NES_DEBUGGER)
    set(SOURCES ${SOURCES} src/debugging/nes_debugger.cpp)
    set(INCLUDES ${INCLUDES} include/debugging/nes_debugger.h src/network/server.cpp)
endif()

add_executable(lbnes main.cpp ${SOURCES} ${INCLUDES})
add_dependencies(lbnes yaml-cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(YAML-CPP_LIB yaml-cppd)
else()
    set(YAML-CPP_LIB yaml-cpp)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(GCC_OPTIMIZATION_FLAGS "-O0")
else()
    set(GCC_OPTIMIZATION_FLAGS "-O3")
endif()

add_definitions(-DIMGUI_RENDERER_SDL)

if(WIN32)
    if(NOT DEFINED ENV{SDL_PATH})
        message(STATUS "Environment variable SDL_PATH is not set, defaulting to C:/SDL2/")
        set(ENV{SDL_PATH} "C:/SDL2/")
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL_LIB_DIRECTORY $ENV{SDL_PATH}/lib/x64)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(SDL_LIB_DIRECTORY $ENV{SDL_PATH}/lib/x86)
    endif()

    target_include_directories(imgui PUBLIC $ENV{SDL_PATH}/include)
    target_link_directories(imgui PUBLIC ${SDL_LIB_DIRECTORY})
    target_link_libraries(imgui PUBLIC SDL2main SDL2)

    target_include_directories(lbnes PUBLIC $ENV{SDL_PATH}/include)
    target_link_directories(lbnes PUBLIC ${SDL_LIB_DIRECTORY})
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes PUBLIC SDL2main SDL2 -l${YAML-CPP_LIB} imgui -lws2_32)

    file(COPY ${SDL_LIB_DIRECTORY}/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    add_definitions(-DSDL_MAIN_HANDLED)
    add_definitions(-DYAML_CPP_STATIC_DEFINE)

    # TODO try to fix this 'hack'
    # Add IFileDialog for mingw64
    add_compile_definitions(NTDDI_VERSION=0x0A000006)
    add_compile_definitions(_WIN32_WINNT=0x0A00)

    add_compile_definitions(UNICODE)
    add_compile_definitions(_UNICODE)

#    Commented at the moment because printing to terminal doesn't work on windows while this is enabled
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-subsystem,windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif(UNIX AND NOT APPLE)
    find_package(SDL2 REQUIRED)
    find_library(SDL2_LIBRARY SDL2)
    include_directories(lbnes "${SDL2_INCLUDE_DIRS}")
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes PUBLIC -lSDL2 -l${YAML-CPP_LIB} imgui)
elseif(APPLE)
    find_library(SDL2_LIBRARY SDL2)
    include_directories(lbnes "${SDL2_INCLUDE_DIRS}/SDL2")
    include_directories(lbnes ${SDL2_INCLUDE_DIRS})
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes PUBLIC ${SDL2_LIBRARY} -l${YAML-CPP_LIB} imgui)
endif()

configure_file(${PROJECT_SOURCE_DIR}/config.yaml ${CMAKE_CURRENT_BINARY_DIR}/config.yaml)

set(CXX_WARNINGS_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS_COMPILE_FLAGS} ${GCC_OPTIMIZATION_FLAGS}")
