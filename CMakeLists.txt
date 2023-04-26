cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER gcc)

include_directories(include)
link_directories(lib)

add_executable(engine
    # Source Files
    src/engine.c
    src/core/core.c
    src/service/machine.c
    src/service/link.c
    src/datatypes/queue.c

    # Include Files
    include/engine.h
    include/core/core.h
    include/service/machine.h
    include/service/link.h
    include/datatypes/queue.h

    # Test Files
    test/test.c
)
if (USING_CLION)
    target_link_libraries(engine librscore.a -lm)
else()
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC MPI::MPI_C librscore.a -lm)
endif()
