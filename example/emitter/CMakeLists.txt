add_executable(emitter main.cpp)

target_link_libraries(emitter PRIVATE piped_console fmt::fmt)
target_compile_features(emitter PRIVATE cxx_std_23)
set_target_properties(emitter PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(emitter PRIVATE UNICODE _UNICODE)
set_target_properties(emitter PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
