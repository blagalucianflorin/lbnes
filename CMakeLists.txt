cmake_minimum_required(VERSION 3.17)
project(lbnes)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/yaml-cpp)

if(NOT DEFINED CMAKE_C_COMPILER)
    message(ERROR "CMAKE_C_COMPILER is not set, defaulting to gcc")
    set(CMAKE_C_COMPILER gcc)
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
    message(ERROR "CMAKE_CXX_COMPILER is not set, defaulting to g++")
    set(CMAKE_CXX_COMPILER g++)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(lbnes ${PROJECT_SOURCE_DIR}/lib/yaml-cpp/include)

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
        src/sdl_manager.cpp
        src/nes.cpp)

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
        include/devices/ppu/oam.h include/devices/inputs/joypad.h
        include/sdl_manager.h
        include/nes.h)

if (NES_DEBUGGER)
    set(SOURCES ${SOURCES} debugging/nes_debugger.cpp)
    set(INCLUDES ${INCLUDES} debugging/nes_debugger.h)
endif ()

add_executable (lbnes main.cpp ${SOURCES} ${INCLUDES})

if(WIN32)
    if(NOT DEFINED ENV{SDL_PATH})
        message(ERROR "Environment variable SDL_PATH is not set, defaulting to C:/SDL2/")
        set($ENV{SDL_PATH} "C:/SDL2/")
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL_LIB_DIRECTORY $ENV{SDL_PATH}/lib/x64)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(SDL_LIB_DIRECTORY $ENV{SDL_PATH}/lib/x86)
    endif()

    target_include_directories(lbnes PUBLIC $ENV{SDL_PATH}/include)
    target_link_directories(lbnes PUBLIC ${SDL_LIB_DIRECTORY})
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes SDL2main SDL2 -lyaml-cppd)

    file(COPY ${SDL_LIB_DIRECTORY}/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    add_definitions(-DSDL_MAIN_HANDLED)
elseif(UNIX AND NOT APPLE)
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes -lSDL2 -lyaml-cppd)
elseif(APPLE)
    find_library(SDL2_LIBRARY SDL2)
    include_directories(lbnes ${SDL2_INCLUDE_DIRS})
    target_link_directories(lbnes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/)
    target_link_libraries(lbnes ${SDL2_LIBRARY} -lyaml-cppd)
endif()

SET (CXX_WARNINGS_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter")
SET (CMAKE_CXX_FLAGS  "${GCC_WARNINGS_COMPILE_FLAGS}")
