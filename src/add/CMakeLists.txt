add_library(Add add.cpp)
set_normal_compile_options(Add)
set_warnings_as_errors(Add)

add_executable(AddTest add_test.cpp)
target_include_directories(AddTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(AddTest PRIVATE Add)
enable_gtest(AddTest)
enable_memcheck(AddTest)
