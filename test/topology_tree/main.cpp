#include <core/core.hpp>
#include <fstream>
#include <model/builder.hpp>
#include <routing/table.hpp>
#include <simulator/simulator.hpp>
#include <string>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>
#include <test.hpp>

#define DEFAULT_ROUTE_FILENAME "topology_tree/routes.route"

extern RoutingTable *g_RoutingTable;

using namespace ispd::sim;

int main(int argc, char **argv)
{
    try {
        // Construct the command-line parser.
        TCLAP::CmdLine cmd("Linear Topology", ' ', "v0.0.1");

        // Argument to specify the amount of cores to use to progress the
        // simulation.
        TCLAP::ValueArg<uint32_t> coresArg(
            "c",
            "cores",
            "Specify the amount of cores to progress the simulation.",
            false,
            0, // @Note: The number '0' indicates all available cores.
            "uint32_t");
        cmd.add(coresArg);

        // Argument to specify the GVT (Global Virtual Time) calculation period.
        TCLAP::ValueArg<uint32_t> gvtPeriodArg(
            "g",
            "gvt",
            "Specify the GVT (Global Virtual Time) calculation period.",
            false,
            1000, // @Note: This value is in microseconds, therefore, it
                  // represents 1ms.
            "microseconds");
        cmd.add(gvtPeriodArg);

        // Argument to specify the checkpointing interval.
        TCLAP::ValueArg<uint32_t> ckptIntervalArg(
            "i",
            "ckpt",
            "Specify the checkpointing interval.",
            false,
            0,
            "uint32_t");
        cmd.add(ckptIntervalArg);

        // Argument to specify the amount of machines to be simulated.
        TCLAP::ValueArg<uint32_t> machineArg(
            "m",
            "machines",
            "Specify the amount of machines linearly linked.",
            false,
            10,
            "uint32_t");
        cmd.add(machineArg);

        // Argument to specify the amount of tasks to be generated.
        TCLAP::ValueArg<uint32_t> taskArg(
            "t",
            "tasks",
            "Specify the amount of tasks to be simulated.",
            false,
            1000,
            "uint32_t");
        cmd.add(taskArg);

        // Argument to specify if the simulation should be executed in the
        // sequential mode.
        TCLAP::SwitchArg serialArg(
            "s",
            "serial",
            "Progress the simulation in the sequential mode.",
            false);
        cmd.add(serialArg);

        // Argument to specify if the thread will be bound to a core.
        TCLAP::SwitchArg coreBindingArg(
            "b", "core-binding", "Enable the thread-to-core binding.", false);
        cmd.add(coreBindingArg);

        // Parse the command-line arguments.
        cmd.parse(argc, argv);

        uint32_t       taskAmount    = taskArg.getValue();
        uint32_t       machineAmount = machineArg.getValue();
        SimulationMode mode = serialArg.getValue() ? SimulationMode::SEQUENTIAL
                                                   : SimulationMode::OPTIMISTIC;

        // Read the routing table from the specified file.
        g_RoutingTable = RoutingTableReader().read(DEFAULT_ROUTE_FILENAME);

        Simulator *s = SimulatorBuilder(SimulatorType::ROOTSIM, mode)
                           .setThreads(coresArg.getValue())
                           .setGvtPeriod(gvtPeriodArg.getValue())
                           .setCoreBinding(coreBindingArg.getValue())
                           .setCheckpointInterval(ckptIntervalArg.getValue())
                           .createSimulator();

        ispd::model::Builder builder(s);

        std::cout << "Start" << std::endl;
        // Register the masters.
        builder.registerMaster(
            0ULL,
            ispd::model::MasterScheduler::ROUND_ROBIN,
            [taskAmount](Master *m) {
                m->addSlave(2ULL);
                m->addSlave(4ULL);

                // Add the master workload to be scheduled.
                ispd::model::workload::zeroth::addConstantSizedWorkload(
                    0ULL, 50.0, 80.0, taskAmount);
                std::cout << "Generated 1" << std::endl;
            });

        builder.registerMaster(2ULL,
                               ispd::model::MasterScheduler::ROUND_ROBIN,
                               [taskAmount](Master *m) {
                                   m->addSlave(6ULL);
                                   m->addSlave(8ULL);
                                   m->addSlave(10ULL);
                                   std::cout << "Generated 2" << std::endl;
                               });

        builder.registerMaster(4ULL,
                               ispd::model::MasterScheduler::ROUND_ROBIN,
                               [taskAmount](Master *m) {
                                   m->addSlave(12ULL);
                                   m->addSlave(14ULL);
                                   m->addSlave(16ULL);
                                   std::cout << "Generated 3" << std::endl;
                               });

        builder.registerMachine(6ULL, 2.0, 0.0, 2);
        builder.registerMachine(8ULL, 2.0, 0.0, 2);
        builder.registerMachine(10ULL, 2.0, 0.0, 2);
        builder.registerMachine(12ULL, 2.0, 0.0, 2);
        builder.registerMachine(14ULL, 2.0, 0.0, 2);
        builder.registerMachine(16ULL, 2.0, 0.0, 2);

        builder.registerLink(1ULL, 0ULL, 2ULL, 5.0, 0.0, 1.0);
        builder.registerLink(3ULL, 0ULL, 4ULL, 5.0, 0.0, 1.0);
        builder.registerLink(5ULL, 2ULL, 6ULL, 5.0, 0.0, 1.0);
        builder.registerLink(7ULL, 2ULL, 8ULL, 5.0, 0.0, 1.0);
        builder.registerLink(9ULL, 2ULL, 10ULL, 5.0, 0.0, 1.0);
        builder.registerLink(11ULL, 4ULL, 12ULL, 5.0, 0.0, 1.0);
        builder.registerLink(13ULL, 4ULL, 14ULL, 5.0, 0.0, 1.0);
        builder.registerLink(15ULL, 14ULL, 16ULL, 5.0, 0.0, 1.0);

        ispd::test::registerMasterServiceFinalizer(s, 0ULL);
        ispd::test::registerMasterServiceFinalizer(s, 2ULL);
        ispd::test::registerMachineServiceFinalizer(s, 6ULL);
        ispd::test::registerMachineServiceFinalizer(s, 16ULL);

        s->simulate();
    }
    catch (const TCLAP::ArgException &e) {
        std::cerr << "Error " << e.error() << " in argument " << e.argId()
                  << "." << std::endl;
    }

    return 0;
}
