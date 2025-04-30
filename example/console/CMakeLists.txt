add_executable(console main.cpp)

target_link_libraries(console PRIVATE piped_console fmt::fmt)
target_compile_features(console PRIVATE cxx_std_23)
set_target_properties(console PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(console PRIVATE UNICODE _UNICODE)
set_target_properties(console PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
