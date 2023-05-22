#include <iostream>
#include <routing/table.hpp>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>
#include <string>

extern RoutingTable *g_RoutingTable;

/**
 * @brief Simulator entry point.
 */
int main(int argc, char **argv)
{
    --argc, ++argv;

    uint64_t machineAmount = 100ULL;
    uint64_t taskAmount    = 1000ULL;

    if (argc > 0) {
        machineAmount = std::stoull(argv[0]);

        // It checks if the specified machine amount is one of the available
        // machine amounts. This is done because the route files only support
        // these machine amounts. However, if new route files is added then
        // different machine amounts also may be used.
        if (machineAmount != 100ULL && machineAmount != 1000ULL &&
            machineAmount != 10000ULL && machineAmount != 100000ULL) {
            std::cout
                << "Machine amount must be one of: 100, 1000, 10000, 100000."
                << std::endl;
            return 0;
        }
    }

    if (argc > 1)
        taskAmount = std::stoull(argv[1]);

    // It reads the routing table from the route file with relation
    // to the number of machines the model has.
    g_RoutingTable = RoutingTableReader().read(
        "model_star/routes_" + std::to_string(machineAmount) + ".route");

    uint64_t   totalLps = machineAmount * 2ULL + 1ULL;
    Simulator *s        = new TimeWarpSimulator();

    for (sid_t id = 0ULL; id < totalLps; id++) {
        /* Initialize the master at id 0 */
        if (id == 0) {
            s->registerService(id, [id, totalLps, taskAmount]() {
                Master *m = ROOTSimAllocator<>::construct<Master>(
                    id, new RoundRobin<sid_t>());

                for (sid_t slaveId = 1ULL; slaveId < totalLps; slaveId += 2)
                    m->addSlave(slaveId);

                timestamp_t jitter = 0.0;

                // Prepare the workload.
                for (unsigned i = 0; i < taskAmount; i++) {
                    Event e(Task(i, 10 + i, 50 + i));
                    ispd::schedule_event(id, jitter, TASK_ARRIVAL, &e, sizeof(e));
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

    s->registerServiceFinalizer(1ULL, [](Service *service) {
        Machine *m = static_cast<Machine *>(service);

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

    s->simulate();

    return 0;
}
