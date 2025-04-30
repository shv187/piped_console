cmake_minimum_required(VERSION 3.25...4.0)

project(piped_console VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_EXAMPLE "Build example" OFF)

include(FetchContent)

FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.1.3)
FetchContent_MakeAvailable(fmtlib)

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
