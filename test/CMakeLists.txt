cmake_minimum_required(VERSION 3.22)
project(test)

set(CMAKE_CXX_STANDARD 17)
find_package(MPI REQUIRED)

file(GLOB SOURCES
        ../include/service/service.hpp
        ../include/engine.hpp
        ../include/core/core.hpp
        ../include/simulator/simulator.hpp
        ../include/simulator/rootsim.hpp
        ../include/customer/customer.hpp
        ../include/event/event.hpp
        ../include/service/machine.hpp
        ../include/service/master.hpp
        ../include/service/link.hpp
        ../include/service/switch.hpp
        ../include/service/dummy.hpp
        ../include/scheduler/round_robin.hpp
        ../include/scheduler/scheduler.hpp
        ../include/workload/workload.hpp
        ../include/workload/distribution.hpp
        ../include/math/distribution/distribution.hpp
        ../include/allocator/rootsim_allocator.hpp
        ../include/routing/table.hpp
        ../include/routing/route.hpp
        ../include/model/builder.hpp
        ../src/core/core.cpp
        ../src/simulator/simulator.cpp
        ../src/simulator/rootsim.cpp
        ../src/service/machine.cpp
        ../src/service/master.cpp
        ../src/service/link.cpp
        ../src/service/switch.cpp
        ../src/model/builder.cpp
        ../src/scheduler/round_robin.cpp
)

function (test_program name)
    add_executable(test_${name} ${ARGN} ${SOURCES})
    include_directories(../src)
    target_include_directories(test_${name} PRIVATE ../include ./include)
    target_link_directories(test_${name} PRIVATE ../lib)
    target_link_libraries(test_${name} MPI::MPI_C librscore.a)
    add_test(NAME test_${name} COMMAND test_${name})
    set_tests_properties(test_${name} PROPERTIES TIMEOUT 60)
endfunction()

enable_testing()

# Topology Tests
test_program(topology_linear topology_linear/main.cpp)
test_program(topology_ring topology_ring/main.cpp)
test_program(topology_star topology_star/main.cpp)
test_program(topology_tree topology_tree/main.cpp)
test_program(topology_star_switched topology_star_switched/main.cpp)

# Distribution Tests
test_program(distribution distribution/main.cpp)
