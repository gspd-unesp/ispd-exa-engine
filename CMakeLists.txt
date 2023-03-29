cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

link_directories(lib)

add_executable(engine src/engine.c include/engine.h)

if (NOT ${USING_CLION} STREQUAL YES)
    message(" -- Building out of CLion")
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC librscore.a MPI::MPI_C -lm)
endif()
