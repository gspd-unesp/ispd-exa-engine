#include <core/core.hpp>
#include <fstream>
#include <model/builder.hpp>
#include <routing/table.hpp>
#include <simulator/simulator.hpp>
#include <string>
#include <tclap/ArgException.h>
#include <tclap/CmdLine.h>
#include <test.hpp>

#define DEFAULT_ROUTE_FILENAME "topology_ring/routes.route"

extern RoutingTable *g_RoutingTable;

using namespace ispd::sim;

/// \brief Create Ring Topology Routing.
///
/// This function generates a routing file for a ring topology model, which
/// includes routes for each machine in the specified number of machines.
///
/// It is worth noting that although the routing table could potentially be
/// created directly without witing to a file and then reading it, this method
/// is used to maintain consistency with simulation cases that typically involve
/// reading a file for the routing table.
///
/// \param filename The name of the routing file to be created.
/// \param machineAmount The total number of machines in the ring topology.
static inline void createRingTopologyRouting(const std::string &filename,
                                             const uint32_t     machineAmount)
{
    std::ofstream routeFile(filename);

    // It checks if the route file could not be opened. If so, the
    // program will be immediately aborted.
    if (!routeFile)
        die("Routing file could not be created.");

    // It calculates the machine highest identifier.
    const uint32_t machineHigherId = machineAmount * 2;
    uint32_t       machineHalfId   = machineHigherId / 2;

    // If the machine half identifier is odd, then this indicantes an imbalance
    // in the number of machines in the first and second halves. Therefore,
    // since there is no odd machine identifier in this model, then the machine
    // half identifier is increased by 1, indicating that the first half will
    // have one more machine than the second one.
    if (machineHalfId % 2)
        machineHalfId++;

    // Write the minimal routing for the first half of machines.
    for (uint32_t machineId = 2; machineId <= machineHalfId; machineId += 2) {
        std::string route;
        // Calculates the route between the master (0) and the curent machine.
        for (uint32_t linkId = 1; linkId < machineId; linkId += 2)
            route += std::to_string(linkId) + " ";

        // Write the route between the master (0) and the current machine.
        routeFile << "0 " << machineId << ' ' << route << '\n';
    }

    // Write the minimal routing for the second half of machines.
    for (uint32_t machineId  = machineHalfId + 2; machineId <= machineHigherId;
         machineId          += 2) {
        std::string route;
        // Calculates the route between the master (0) and the curent machine.
        for (uint32_t linkId  = machineHigherId + 1; linkId > machineId;
             linkId          -= 2)
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
        TCLAP::CmdLine cmd("Ring Topology", ' ', "v0.0.1");

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

        TCLAP::ValueArg<uint32_t> gvtPeriodArg(
            "g",
            "gvt",
            "Specify the GVT (Global Virtual Time) calculation period.",
            false,
            1000, // @Note: This value is in microseconds, therefore, it
                  // represents 1ms.
            "microseconds");
        cmd.add(gvtPeriodArg);

        // Argument to specify the amount of machines to be simulated.
        TCLAP::ValueArg<uint32_t> machineArg(
            "m",
            "machines",
            "Specify the amount of machines linked in a ring.",
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

        // A ring topology can not be created with only one master and machine
        // in the topology. Therefore, it must have more than only one machine
        // for close the ring.
        if (machineAmount <= 1)
            die("Machine amount should be greater than 1.\n");

        createRingTopologyRouting(DEFAULT_ROUTE_FILENAME, machineAmount);

        // Read the routing table from the specified file.
        g_RoutingTable = RoutingTableReader().read(DEFAULT_ROUTE_FILENAME);

        Simulator *s = SimulatorBuilder(SimulatorType::ROOTSIM, mode)
                           .cores(coresArg.getValue())
                           .gvtPeriod(gvtPeriodArg.getValue())
                           .coreBinding(coreBindingArg.getValue())
                           .createSimulator();

        ispd::model::Builder builder(s);

        // Calculates the machine with the highest identifier.
        const uint32_t machineHigherId = machineAmount * 2UL;
        uint32_t       machineHalfId   = machineHigherId / 2UL;

        if (machineHalfId % 2)
            machineHalfId++;

        // Register the master.
        builder.registerMaster(
            0ULL,
            ispd::model::MasterScheduler::ROUND_ROBIN,
            [taskAmount, machineHigherId](Master *m) {
                // Add the slaves.
                for (uint32_t machineId  = 2UL; machineId <= machineHigherId;
                     machineId          += 2UL)
                    m->addSlave(machineId);

                // Add the master workload to be scheduled.
                ispd::model::workload::zeroth::addConstantSizedWorkload(
                    0ULL, 50.0, 80.0, taskAmount);
            });

        // Register the machines and links.
        for (uint32_t machineId  = 2UL; machineId <= machineHigherId;
             machineId          += 2UL) {
            const uint32_t linkId = machineId - 1UL;

            builder.registerMachine(machineId, 2.0, 0.0, 2);
            builder.registerLink(
                linkId, linkId - 1UL, linkId + 1UL, 5.0, 0.0, 1.0);
        }

        // Register the last link to close the ring.
        builder.registerLink(
            machineHigherId + 1UL, machineHigherId, 0ULL, 5.0, 0.0, 1.0);

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
