cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

link_directories(lib)


function(test_program name)
    add_executable(test_${name} ${ARGN})
    add_test(NAME test_${name} COMMAND test_${name})
endfunction()

if (NOT ${TEST} STREQUAL YES)
    add_executable(engine
            # Source Files
            src/engine.c
            src/mm/mm.c
            src/core/core.c
            src/service/link.c
            src/service/machine.c

            # Include Files
            include/engine.h
            include/mm/mm.h
            include/core/core.h
            include/service/machine.h
            include/service/link.h
            include/datatypes/array.h
            include/datatypes/queue.h

            # Test Files
            test/test.c
            )
else()
    test_program(queue test/datatypes/queue.c)
endif()

if (NOT ${USING_CLION} STREQUAL YES)
    message(" -- Building out of CLion")
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC librscore.a MPI::MPI_C -lm)
endif()