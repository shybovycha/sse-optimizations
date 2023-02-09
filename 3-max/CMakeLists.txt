add_executable(3-max main.cpp)

target_link_libraries(3-max PUBLIC common)

if(MSVC)
    target_compile_options(3-max INTERFACE /arch:AVX512)
else()
    target_compile_options(3-max INTERFACE -mavx)
endif()
