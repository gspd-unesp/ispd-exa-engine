#include "../test.hpp"
#include <model/builder.hpp>
#include <routing/table.hpp>
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
    bool     jittered      = false;

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
    if (argc > 2)
        jittered = std::string(argv[2]) == "yes";

    // It reads the routing table from the route file with relation
    // to the number of machines the model has.
    g_RoutingTable = RoutingTableReader().read(
        "model_star/routes_" + std::to_string(machineAmount) + ".route");

    uint64_t             totalLps = machineAmount * 2ULL + 1ULL;
    Simulator           *s        = new TimeWarpSimulator();
    ispd::model::Builder builder(s);

    builder.registerMaster(
        0ULL,
        ispd::model::MasterScheduler::ROUND_ROBIN,
        [totalLps, taskAmount](Master *m) {
            for (sid_t slaveId = 1ULL; slaveId < totalLps; slaveId += 2)
                m->addSlave(slaveId);

            ispd::model::workload::zeroth::addConstantSizedWorkload(
                0ULL, 10.0, 50.0, taskAmount);
        });

    for (sid_t id = 1ULL; id < totalLps; id++) {
        /* Initialize the machines at odd identifiers */
        if ((id % 2) == 1) {
            builder.registerMachine(id, 2.0, 0.0, 2);
        }
        /* Initialize the links at even identifiers */
        else {
            builder.registerLink(id, 0ULL, id - 1, 5.0, 0.0, 1.0);
        }
    }

    ispd::test::registerMasterServiceFinalizer(s, 0ULL);
    ispd::test::registerMachineServiceFinalizer(s, 1ULL);

    s->simulate();

    return 0;
}
