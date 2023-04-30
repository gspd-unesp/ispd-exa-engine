cmake_minimum_required(VERSION 3.22)
project(engine)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib)
add_executable(engine
        # Include Files
        include/service/service.hpp
        include/engine.hpp
        include/core/core.hpp
        include/simulator/simulator.hpp
        include/simulator/timewarp.hpp
        include/customer/customer.hpp
        include/event/event.hpp
        include/service/machine.hpp
        include/service/master.hpp
        include/service/link.hpp
        include/scheduler/round_robin.hpp
        include/scheduler/scheduler.hpp

        # Source Files
        main.cpp
        src/core/core.cpp
        src/simulator/timewarp.cpp
        src/service/machine.cpp
        src/service/master.cpp
        src/service/link.cpp
        )
if (${USING_CLION} EQUAL 1)
    target_link_libraries(engine librscore.a -lm)
else()
    find_package(MPI REQUIRED)
    target_link_libraries(engine PUBLIC MPI::MPI_C librscore.a -lm)
endif()