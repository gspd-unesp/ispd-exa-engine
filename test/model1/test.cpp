#include "../test.hpp"
#include <model/builder.hpp>
#include <routing/table.hpp>
#include <simulator/timewarp.hpp>

extern RoutingTable *g_RoutingTable;

int main(int argc, char **argv)
{
    --argc, ++argv;

    if (0 == argc)
        die("A route file has not been specified.");
    else
        g_RoutingTable = RoutingTableReader().read(argv[0]);

    uint64_t taskAmount = 1000ULL;
    bool     jittered   = false;

    if (argc > 1)
        taskAmount = std::stoull(argv[1]);
    if (argc > 2)
        jittered = std::string(argv[2]) == "yes";

    Simulator           *s = new TimeWarpSimulator();
    ispd::model::Builder builder(s);

    builder.registerMaster(
        0ULL,
        ispd::model::MasterScheduler::ROUND_ROBIN,
        [taskAmount, jittered](Master *m) {
            m->addSlave(2ULL);
            ispd::model::workload::zeroth::addConstantSizedWorkload(
                0ULL, 50.0, 80.0, taskAmount, jittered);
        });
    builder.registerLink(1ULL, 0ULL, 2ULL, 5.0, 0.0, 1.0);
    builder.registerMachine(2ULL, 2.0, 0.0, 2);

    ispd::test::registerMasterServiceFinalizer(s, 0ULL);
    ispd::test::registerMachineServiceFinalizer(s, 2ULL);

    s->simulate();

    return 0;
}
