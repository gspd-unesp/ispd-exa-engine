#include <allocator/rootsim_allocator.hpp>
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
        m->addSlave(4ULL);
        m->addSlave(6ULL);

        for (uint64_t i = 0ULL; i < taskAmount; i++) {
            Event e(Task(i, 50.0, 80.0));
            schedule_event(0ULL, 0.0, TASK_ARRIVAL, &e, sizeof(e));
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

    s->registerService(3ULL, []() {
        Link *l = ROOTSimAllocator<>::construct<Link>(
            3ULL, 0ULL, 4ULL, 5.0, 0.0, 1.0);
        return l;
    });

    s->registerService(4ULL, []() {
        Machine *m = ROOTSimAllocator<>::construct<Machine>(4ULL, 2.0, 0.0, 2);
        return m;
    });

    s->registerService(5ULL, []() {
        Link *l = ROOTSimAllocator<>::construct<Link>(
            5ULL, 4ULL, 6ULL, 5.0, 0.0, 1.0);
        return l;
    });

    s->registerService(6ULL, []() {
        Machine *m = ROOTSimAllocator<>::construct<Machine>(6ULL, 2.0, 0.0, 2);
        return m;
    });

    s->registerService(7ULL, []() {
        Link *l = ROOTSimAllocator<>::construct<Link>(
            7ULL, 2ULL, 6ULL, 5.0, 0.0, 1.0);
        return l;
    });

    for (sid_t i = 2ULL; i <= 6ULL; i += 2ULL) {
        s->registerServiceFinalizer(i, [](Service *service) {
            Machine *m = (Machine *)service;

            std::cout << "Machine Metrics\n" << std::endl;
            std::cout << " - LVT: " << m->getLocalVirtualTime() << " @ LP ("
                      << m->getId() << ")" << std::endl;
            std::cout << " - Processed MFlops: " << m->getMetrics().m_ProcMFlops
                      << " @ LP (" << m->getId() << ")" << std::endl;
            std::cout << " - Processed Time: " << m->getMetrics().m_ProcTime
                      << " @ LP (" << m->getId() << ")" << std::endl;
            std::cout << " - Processed Tasks: " << m->getMetrics().m_ProcTasks
                      << " @ LP (" << m->getId() << ")" << std::endl;
            std::cout << std::endl;
        });
    }

    s->simulate();

    return 0;
}