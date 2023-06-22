#include <core/core.hpp>
#include <fstream>
#include <model/builder.hpp>
#include <routing/table.hpp>
#include <simulator/simulator.hpp>
#include <string>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>
#include <test.hpp>

#define DEFAULT_ROUTE_FILENAME "topology_linear/routes.route"

extern RoutingTable *g_RoutingTable;

using namespace ispd::sim;

/// \brief Create Linear Topology Routing.
///
/// This function generates a routing file for a linear topology model, which
/// includes routes for each machine in the specified number of machines.
///
/// It is worth noting that although the routing table could potentially be
/// created directly without witing to a file and then reading it, this method
/// is used to maintain consistency with simulation cases that typically involve
/// reading a file for the routing table.
///
/// \param filename The name of the routing file to be created.
/// \param machineAmount The total number of machines in the linear topology.
static inline void createLinearTopologyRouting(const std::string &filename,
                                               const uint32_t     machineAmount)
{
    std::ofstream routeFile(filename);

    // It checks if the route file could not be opened. If so, the
    // program will be immediately aborted.
    if (!routeFile)
        die("Routing file could not be created.");

    // It calculates the machine highest identifier.
    const uint32_t machineHigherId = machineAmount * 2;

    for (uint32_t machineId = 2; machineId <= machineHigherId; machineId += 2) {
        std::string route;
        // Calculates the route between the master (0) and the curent machine.
        for (uint32_t linkId = 1; linkId < machineId; linkId += 2)
            route += std::to_string(linkId) + " ";

        // Write the route between the master (0) and the current machine.
        routeFile << "0 " << machineId << ' ' << route << '\n';
    }

    routeFile.close();
}

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

        createLinearTopologyRouting(DEFAULT_ROUTE_FILENAME, machineAmount);

        // Read the routing table from the specified file.
        g_RoutingTable = RoutingTableReader().read(DEFAULT_ROUTE_FILENAME);

        Simulator *s = SimulatorBuilder(SimulatorType::ROOTSIM, mode)
                           .setThreads(coresArg.getValue())
                           .setGvtPeriod(gvtPeriodArg.getValue())
                           .setCoreBinding(coreBindingArg.getValue())
                           .setCheckpointInterval(ckptIntervalArg.getValue())
                           .createSimulator();

        ispd::model::Builder builder(s);

        // Calculates the machine with the highest identifier.
        const uint32_t machineHigherId = machineAmount * 2UL;

        // Register the master.
        builder.registerMaster(
            0ULL,
            ispd::model::MasterScheduler::ROUND_ROBIN,
            [taskAmount, machineHigherId](Master *m) {
                /// @Test: This is temporary.
                m->m_Workload =
                    ROOTSimAllocator<>::construct<UniformRandomWorkload>(
                        taskAmount, 10.0, 15.0, 20.0, 50.0);

                // Add the slaves.
                for (uint32_t machineId  = 2UL; machineId <= machineHigherId;
                     machineId          += 2UL)
                    m->addSlave(machineId);

                /// It sends an event to the master to indicate that its
                /// scheduling algorithm should be initialized.
                ispd::schedule_event(
                    m->getId(), 0.0, TASK_SCHEDULER_INIT, nullptr, 0);
            });

        // Register the machines in the linear topology model.
        for (uint32_t machineId  = 2UL; machineId <= machineHigherId;
             machineId          += 2UL)
            builder.registerMachine(machineId, 2.0, 0.0, 2);

        // Register links that links the machine in a linear linked manner.
        for (uint32_t linkId = 1UL; linkId < machineHigherId; linkId += 2UL)
            builder.registerLink(
                linkId, linkId - 1ULL, linkId + 1ULL, 5.0, 0.0, 1.0);

        ispd::test::registerMasterServiceFinalizer(s, 0ULL);
        ispd::test::registerMachineServiceFinalizer(s, 2ULL);

        s->simulate();
    }
    catch (const TCLAP::ArgException &e) {
        std::cerr << "Error " << e.error() << " in argument " << e.argId()
                  << "." << std::endl;
    }

    return 0;
}
