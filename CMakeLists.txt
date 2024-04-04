cmake_minimum_required(VERSION 3.12)

project(iciba VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 13)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLSGS "-03 -Wall -Wextra")

add_subdirectory(src)
add_subdirectory(package)
