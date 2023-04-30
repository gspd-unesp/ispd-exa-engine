#include <iostream>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <workload/workload.hpp>

/**
 * @brief Simulator entry point.
 */
int main(int argc, char **argv)
{
    --argc, ++argv;

    /*
     * The test case being simulated is a
     * star topology containing
     *
     *   - 1 master
     *   - 8000 machines
     *   - 8000 links
     *
     * with a workload containing 2 million tasks.
     */
    Simulator *s = new TimeWarpSimulator();

    s->registerService(0ULL, []() {
        Master      *m = new Master(0ULL, new RoundRobin<sid_t>());
        NodeWorkload workload(0ULL, 10.0, 15.0, 5.0, 8.0);

        // Add link.
        m->addLink(1ULL);

        for (int i = 0; i < 100; i++) {
            timestamp_t arrival_time;

            // Generate the tas using the workload generator.
            Event e(workload(arrival_time));

            // Schedule the task.
            schedule_event(0ULL, arrival_time, TASK_ARRIVAL, &e, sizeof(e));
        }

        return m;
    });

    s->registerService(1ULL, []() {
        Link *l = new Link(1LL, 0ULL, 2ULL, 5.0, 0.0, 1.0);
        return l;
    });

    s->registerService(2ULL, []() {
        Machine *m = new Machine(2LL, 2.0, 0.0, 2);
        return m;
    });

    s->simulate();

    return 0;
}
