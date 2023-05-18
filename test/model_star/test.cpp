#include <iostream>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <string>

/**
 * @brief Simulator entry point.
 */
int main(int argc, char **argv)
{
    --argc, ++argv;

    uint64_t machineAmount = 100ULL;
    uint64_t taskAmount    = 1000ULL;

    // It gets the amount of machine in the star topology 
    // from the command-line argument.
    if (argc > 0)
        machineAmount = std::stoull(argv[0]);

    // It gets the amount of tasks for the master schedule
    // from the command-line argument.
    if (argc > 1) 
        taskAmount = std::stoull(argv[1]);

    uint64_t totalLps = machineAmount * 2ULL + 1ULL;

    /*
     * The test case being simulated is a
     * star topology containing
     *
     *   - 1 master
     *   - 25000 machines
     *   - 25000 links
     *
     * with a workload containing 2 million tasks.
     */
    Simulator *s = new TimeWarpSimulator();

    for (sid_t id = 0ULL; id < totalLps; id++) {
        /* Initialize the master at id 0 */
        if (id == 0) {
            s->registerService(id, [id, totalLps, taskAmount]() {
                Master *m = ROOTSimAllocator<>::construct<Master>(
                    id, new RoundRobin<sid_t>());

                // Add the links to the master.
                for (sid_t linkId = 2ULL; linkId < totalLps; linkId += 2)
                    m->addLink(linkId);

                timestamp_t jitter = 0.0;

                // Prepare the workload.
                for (unsigned i = 0; i < taskAmount; i++) {
                    Event e(Task(i, 10 + i, 50 + i));
                    schedule_event(id, jitter, TASK_ARRIVAL, &e, sizeof(e));
                    jitter += 1e-52;
                }

                return m;
            });
        }
        /* Initialize the machines at odd identifiers */
        else if ((id % 2) == 1) {
            s->registerService(id, [id]() {
                Machine *m =
                    ROOTSimAllocator<>::construct<Machine>(id, 2.0, 0.0, 2);
                return m;
            });
        }
        /* Initialize the links at even identifiers */
        else {
            s->registerService(id, [id]() {
                Link *l = ROOTSimAllocator<>::construct<Link>(
                    id, 0ULL, id - 1, 5.0, 0.0, 1.0);
                return l;
            });
        }
    }

    s->simulate();

    return 0;
}
