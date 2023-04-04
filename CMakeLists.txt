cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

link_directories(lib)

add_executable(engine
        # Source Files
        src/engine.c
        src/mm/mm.c
        src/core/core.c
        src/service/link.c
        src/service/machine.c
        src/main.c

        # Include Files
        include/engine.h
        include/mm/mm.h
        include/core/core.h
        include/service/machine.h
        include/service/link.h
        include/datatypes/queue.h
        )

if (NOT ${USING_CLION} STREQUAL YES)
    message(" -- Building out of CLion")
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC librscore.a MPI::MPI_C -lm)
endif()
