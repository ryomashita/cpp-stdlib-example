add_subdirectory(add)

function(build_directory target dirpath)
    file(GLOB_RECURSE SRC_LIST "${dirpath}/*.cpp")
    add_executable(${target} ${SRC_LIST})
    set_normal_compile_options(${target})
    set_warnings_as_errors(${target})
    enable_gtest(${target})
endfunction()

# targets
build_directory(span span)
