cmake_minimum_required(VERSION 3.22)
project(test)

find_package(MPI REQUIRED)

file(GLOB SOURCES
        ../include/service/service.hpp
        ../include/engine.hpp
        ../include/core/core.hpp
        ../include/simulator/simulator.hpp
        ../include/simulator/timewarp.hpp
        ../include/customer/customer.hpp
        ../include/event/event.hpp
        ../include/service/machine.hpp
        ../include/service/master.hpp
        ../include/service/link.hpp
        ../include/scheduler/round_robin.hpp
        ../include/scheduler/scheduler.hpp
        ../include/workload/workload.hpp
        ../include/math/distribution/distribution.hpp
        ../include/allocator/rootsim_allocator.hpp
        ../include/routing/table.hpp
        ../src/core/core.cpp
        ../src/simulator/timewarp.cpp
        ../src/service/machine.cpp
        ../src/service/master.cpp
        ../src/service/link.cpp
)

function (test_program name)
    add_executable(test_${name} ${ARGN} ${SOURCES})
    include_directories(../src)
    target_include_directories(test_${name} PRIVATE ../include .)
    target_link_directories(test_${name} PRIVATE ../lib)
    target_link_libraries(test_${name} MPI::MPI_C librscore.a)
    add_test(NAME test_${name} COMMAND test_${name})
    set_tests_properties(test_${name} PROPERTIES TIMEOUT 60)
endfunction()

enable_testing()

test_program(model1 model1/test.cpp)
test_program(model2 model2/test.cpp)
test_program(model_star model_star/test.cpp)
test_program(model_route model_route/test.cpp)
test_program(model_ring model_ring/test.cpp)
