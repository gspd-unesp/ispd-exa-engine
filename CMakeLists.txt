cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)
add_executable(engine
        # Include Files
        include/engine.hpp
        include/core/core.hpp
        include/simulator/simulator.hpp
        include/simulator/rootsim.hpp
        include/customer/customer.hpp
        include/event/event.hpp
        include/service/service.hpp
        include/service/machine.hpp
        include/service/master.hpp
        include/service/link.hpp
        include/service/dummy.hpp
        include/service/switch.hpp
        include/scheduler/round_robin.hpp
        include/scheduler/scheduler.hpp
        include/workload/workload.hpp
        include/math/distribution/distribution.hpp
        include/allocator/rootsim_allocator.hpp
        include/routing/table.hpp
        include/routing/route.hpp
        include/model/builder.hpp


        # Source Files
        main.cpp
        src/core/core.cpp
        src/simulator/simulator.cpp
        src/simulator/rootsim.cpp
        src/service/machine.cpp
        src/service/master.cpp
        src/service/link.cpp
        src/service/switch.cpp
        src/model/builder.cpp
        )

if (DEFINED CLION)
    target_link_libraries(engine librscore.a -lm)
else()
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC MPI::MPI_C librscore.a -lm)
endif()
