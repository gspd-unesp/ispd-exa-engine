#include "../test.hpp"
#include <allocator/rootsim_allocator.hpp>
#include <iostream>
#include <routing/table.hpp>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <string>

#define NEW_MACHINE(id) ROOTSimAllocator<>::construct<Machine>(id, 2.0, 0.0, 2);
#define NEW_LINK(id, from, to)                                                 \
    ROOTSimAllocator<>::construct<Link>(id, from, to, 5.0, 0.0, 1.0);

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
        m->addSlave(4ULL);
        m->addSlave(6ULL);
        m->addSlave(8ULL);

        for (uint64_t i = 0ULL; i < taskAmount; i++) {
            Event e(Task(i, 50.0, 80.0));
            ispd::schedule_event(0ULL, 0.0, TASK_ARRIVAL, &e, sizeof(e));
        }

        return m;
    });

    s->registerService(1ULL, []() { return NEW_LINK(1ULL, 0ULL, 2ULL); });
    s->registerService(2ULL, []() { return NEW_MACHINE(2ULL); });
    s->registerService(3ULL, []() { return NEW_LINK(3ULL, 2ULL, 4ULL); });
    s->registerService(4ULL, []() { return NEW_MACHINE(4ULL); });
    s->registerService(5ULL, []() { return NEW_LINK(5ULL, 4ULL, 6ULL); });
    s->registerService(6ULL, []() { return NEW_MACHINE(6ULL); });
    s->registerService(7ULL, []() { return NEW_LINK(7ULL, 6ULL, 8ULL); });
    s->registerService(8ULL, []() { return NEW_MACHINE(8ULL); });
    s->registerService(9ULL, []() { return NEW_LINK(9ULL, 8ULL, 0ULL); });

    ispd::test::registerMasterServiceFinalizer(s, 0ULL);
    for (sid_t machineId = 2ULL; machineId < 9ULL; machineId += 2ULL)
        ispd::test::registerMachineServiceFinalizer(s, machineId);

    s->simulate();

    return 0;
}
