add_subdirectory(add)

add_executable(span span/basic.cpp)
set_normal_compile_options(span)
enable_gtest(span)