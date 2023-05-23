#ifndef ENGINE_TEST_HPP
#define ENGINE_TEST_HPP

#include <core/core.hpp>
#include <cstdio>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/simulator.hpp>

namespace ispd
{
namespace test
{
/**
 * @brief It register a master service finalizer that is used to print
 * 	  the master's statistics at the end of the simulation for the master
 * 	  with the specified service identifier.
 *
 * @param simulator the simulator in which the service finalizer will
 * 		    be registered to
 * @param serviceId the service identifier
 */
inline void registerMasterServiceFinalizer(Simulator *const simulator,
                                           const sid_t      serviceId)
{
    // It checks if the simulator has not been specified. If so, then
    // the program will be aborted immediately.
    if (not simulator)
        die("registerMachineServiceFinalizer: Simulator is NULL");

    simulator->registerServiceFinalizer(serviceId, [](Service *service) {
        const Master        *m       = static_cast<Master *>(service);
        const MasterMetrics &metrics = m->getMetrics();
        std::printf("Master Metrics\n"
                    " - Last Activity Time: %lf @ LP (%lu)\n"
                    " - Completed Tasks: %u @ LP (%lu)\n\n",
                    metrics.m_LastActivityTime,
                    m->getId(),
                    metrics.m_CompletedTasks,
                    m->getId());
    });
}
/**
 * @brief It register a machine service finalizer that is used to print
 *    	  the machine's statistics at the end of the simulation for the machine
 *    	  with the specified service identifier.
 *
 * @param simulator the simulator in which the service finalizer will be
 * 		    registered to
 * @param serviceId the service identifier
 */
inline void registerMachineServiceFinalizer(Simulator *const simulator,
                                            const sid_t      serviceId)
{
    // It checks if the simulator has not been specified. If so, then
    // the program will be aborted immediately.
    if (not simulator)
        die("registerMachineServiceFinalizer: Simulator is NULL");

    simulator->registerServiceFinalizer(serviceId, [](Service *service) {
        const Machine        *m       = static_cast<Machine *>(service);
        const MachineMetrics &metrics = m->getMetrics();

        std::printf("Machine Metrics\n"
                    " - Last Activity Time: %lf @ LP (%lu)\n"
                    " - Processed MFLOPS..: %lf @ LP (%lu)\n"
                    " - Processed Time....: %lf @ LP (%lu)\n"
                    " - Processed Tasks...: %d @ LP (%lu)\n"
                    " - Forwarded Packets.: %u @ LP (%lu)\n"
                    "\n",
                    metrics.m_LastActivityTime,
                    m->getId(),
                    metrics.m_ProcMFlops,
                    m->getId(),
                    metrics.m_ProcTime,
                    m->getId(),
                    metrics.m_ProcTasks,
                    m->getId(),
                    metrics.m_ForwardedPackets,
                    m->getId());
    });
}

} // namespace test
} // namespace ispd
#endif // ENGINE_TEST_HPP
