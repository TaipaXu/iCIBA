cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(iciba VERSION 0.3.0 LANGUAGES CXX)

option(ENABLE_FORMAT "Enable std::format support" OFF)

if(ENABLE_FORMAT)
    message(STATUS "std::format is enabled")
    add_compile_definitions(ENABLE_FORMAT_SUPPORT)
    set(CMAKE_CXX_STANDARD 23)
else()
    message(STATUS "std::format is disabled")
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
add_subdirectory(package)
