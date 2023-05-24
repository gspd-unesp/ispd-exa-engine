#include "../test.hpp"
#include <allocator/rootsim_allocator.hpp>
#include <cstdio>
#include <iostream>
#include <routing/table.hpp>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <string>

extern RoutingTable *g_RoutingTable;

int main(int argc, char **argv)
{
    --argc, ++argv;

    if (0 == argc)
        die("A route file has not been specified.");
    else
        g_RoutingTable = RoutingTableReader().read(argv[0]);

    uint64_t taskAmount = 1000ULL;
    if (argc > 1)
        taskAmount = std::stoull(argv[1]);

    Simulator *s = new TimeWarpSimulator();
    s->registerService(0ULL, [taskAmount]() {
        Master *m = ROOTSimAllocator<>::construct<Master>(
            0ULL, ROOTSimAllocator<>::construct<RoundRobin<sid_t>>());

        m->addSlave(2ULL);

        timestamp_t jitter = 0.0;

        for (uint64_t i = 0ULL; i < taskAmount; i++) {
            Event e(Task(i, 50.0, 80.0));
            ispd::schedule_event(0ULL, jitter, TASK_ARRIVAL, &e, sizeof(e));
            jitter += 1e-52;
        }

        return m;
    });

    s->registerService(1ULL, []() {
        Link *l = ROOTSimAllocator<>::construct<Link>(
            1ULL, 0ULL, 2ULL, 5.0, 0.0, 1.0);
        return l;
    });

    s->registerService(2ULL, []() {
        Machine *m = ROOTSimAllocator<>::construct<Machine>(2ULL, 2.0, 0.0, 2);
        return m;
    });

    ispd::test::registerMasterServiceFinalizer(s, 0ULL);
    ispd::test::registerMachineServiceFinalizer(s, 2ULL);

    s->simulate();

    return 0;
}
