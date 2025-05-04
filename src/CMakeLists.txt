add_library(piped_console STATIC
    piped_console.cpp
)

target_link_libraries(piped_console PRIVATE fmt::fmt)
target_include_directories(piped_console PUBLIC ../include)
target_compile_features(piped_console PUBLIC cxx_std_23)
set_target_properties(piped_console PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(piped_console PUBLIC UNICODE _UNICODE WIN32_LEAN_AND_MEAN)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(piped_console PRIVATE /W4 /WX /MP)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(piped_console PRIVATE -Wall -Wextra -Werror -pedantic)
else()
    message(FATAL_ERROR "Invalid compiler")
endif()
