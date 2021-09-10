cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lib/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(midi2snd)

find_package(SndFile CONFIG REQUIRED)

add_executable(midi2snd main.cpp)
target_link_libraries(midi2snd PRIVATE SndFile::sndfile)
target_include_directories(midi2snd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinysoundfont)
