#include <iostream>
#include <allocator/rootsim_allocator.hpp>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <string>

int main(int argc, char **argv)
{
    --argc, ++argv;

    uint64_t taskAmount = 1000ULL;
    if (argc > 0)
        taskAmount = std::stoull(argv[0]);

    Simulator *s = new TimeWarpSimulator();
    s->registerService(0ULL, [taskAmount]() {
        Master *m = ROOTSimAllocator<>::construct<Master>(
            0ULL, ROOTSimAllocator<>::construct<RoundRobin<sid_t>>());

        m->addLink(1ULL);

        timestamp_t jitter = 0.0;

        for (uint64_t i = 0ULL; i < taskAmount; i++) {
            Event e(Task(i, 50.0, 80.0));
            schedule_event(0ULL, jitter, TASK_ARRIVAL, &e, sizeof(e));
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


    s->registerServiceFinalizer(2ULL, [](Service *service) {
        Machine *m = (Machine *)service;

        std::cout << "Machine Metrics\n" << std::endl;
        std::cout << " - LVT: " << m->getLocalVirtualTime() << " @ LP (" << m->getId() << ")" << std::endl;
        std::cout << " - Processed MFlops: " << m->getMetrics().m_ProcMFlops << " @ LP (" << m->getId() << ")" << std::endl;
        std::cout << " - Processed Time: " << m->getMetrics().m_ProcTime << " @ LP (" << m->getId() << ")" << std::endl;
        std::cout << " - Processed Tasks: " << m->getMetrics().m_ProcTasks << " @ LP (" << m->getId() << ")" << std::endl;
        std::cout << std::endl;

    });

    s->simulate();

    return 0;
}