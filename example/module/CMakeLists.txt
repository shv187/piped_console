add_library(module SHARED dllmain.cpp)

target_link_libraries(module PRIVATE piped_console fmt::fmt)
target_compile_features(module PRIVATE cxx_std_23)
set_target_properties(module PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(module PRIVATE UNICODE _UNICODE)
set_target_properties(module PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
