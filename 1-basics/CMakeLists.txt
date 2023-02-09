add_executable(1-basics main.cpp)

if(MSVC)
    target_compile_options(1-basics INTERFACE /arch:AVX512)
else()
    target_compile_options(1-basics INTERFACE -mavx)
endif()
