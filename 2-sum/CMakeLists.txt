add_executable(2-sum main.cpp)

target_link_libraries(2-sum PUBLIC common)

if(MSVC)
    target_compile_options(2-sum INTERFACE /arch:AVX512)
else()
    target_compile_options(2-sum INTERFACE -mavx)
endif()
