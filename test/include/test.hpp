#ifndef ENGINE_TEST_HPP
#define ENGINE_TEST_HPP

#include <core/core.hpp>
#include <cstdio>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <service/switch.hpp>
#include <simulator/simulator.hpp>

namespace ispd
{
namespace test
{
/// \brief Registers a master service finalizer that is used to print
///        the master's statistics at the end of the simulation for the master
///        with the specified service identifier.
///
/// \param simulator The simulator to which the service finalizer will be
///                   registered.
/// \param serviceId The service identifier of the master.
///
/// \note If the simulator is not specified (null), the program will be aborted.
///
/// \details The master service finalizer is a lambda function that is called by
///          the simulator at the end of the simulation for the specified master
///          service. It retrieves the metrics of the master, including the last
///          activity time and the number of completed tasks, and prints them to
///          the console in a formatted manner.
inline void registerMasterServiceFinalizer(
    ispd::sim::Simulator *const simulator, const sid_t serviceId)
{
    /// It checks if the simulator has not been specified. If so, then
    /// the program will be aborted immediately.
    if (not simulator)
        die("registerMachineServiceFinalizer: Simulator is NULL");

    simulator->registerServiceFinalizer(serviceId, [](Service *service) {
        const Master        *m       = static_cast<Master *>(service);
        const MasterMetrics &metrics = m->getMetrics();

        /// Print the master metrics.
        std::printf("Master Metrics\n"
                    " - Last Activity Time: %lf @ LP (%lu)\n"
                    " - Completed Tasks: %u @ LP (%lu)\n\n",
                    metrics.m_LastActivityTime,
                    m->getId(),
                    metrics.m_CompletedTasks,
                    m->getId());
    });
}

/// \brief Registers a machine service finalizer that is used to print
///        the machine's statistics at the end of the simulation for the machine
///        with the specified service identifier.
///
/// \param simulator The simulator to which the service finalizer will be
///                  registered.
/// \param serviceId The service identifier of the machine.
///
/// \note If the simulator is not specified (null), the program will be aborted.
///
/// \details The machine service finalizer is a lambda function that is called
///          by the simulator at the end of the simulation for the specified
///          machine service. It retrieves the metrics of the machine, including
///          the last activity time, processed MFLOPS, processed time, processed
///          tasks, and forwarded packets, and prints them to the console in a
///          formatted manner.
inline void registerMachineServiceFinalizer(
    ispd::sim::Simulator *const simulator, const sid_t serviceId)
{
    /// It checks if the simulator has not been specified. If so, then
    /// the program will be aborted immediately.
    if (not simulator)
        die("registerMachineServiceFinalizer: Simulator is NULL");

    simulator->registerServiceFinalizer(serviceId, [](Service *service) {
        const Machine        *m       = static_cast<Machine *>(service);
        const MachineMetrics &metrics = m->getMetrics();

        /// Print the machine metrics.
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

inline void registerSwitchServiceFinalizer(
    ispd::sim::Simulator *const simulator, const sid_t serviceId)
{
    /// It checks if the simulator has not been specified. If so, then
    /// the program will be aborted immediately.
    if (not simulator)
        die("registerSwitchServiceFinalizer: Simulator is NULL");

    simulator->registerServiceFinalizer(serviceId, [](Service *service) {
        const Switch        *s       = static_cast<Switch *>(service);
        const SwitchMetrics &metrics = s->getMetrics();

        /// Print the switch metrics.
        std::printf("Switch Metrics\n"
                    " - Last Activity Time..: %lf @ LP (%lu)\n"
                    " - Communicated Mbits..: %lf @ LP (%lu)\n"
                    " - Communicated Time...: %lf @ LP (%lu)\n"
                    " - Communicated Packets: %u @ LP (%lu)\n"
                    "\n",
                    metrics.m_LastActivityTime,
                    s->getId(),
                    metrics.m_CommMBits,
                    s->getId(),
                    metrics.m_CommTime,
                    s->getId(),
                    metrics.m_CommPackets,
                    s->getId());
    });
}

} // namespace test
} // namespace ispd
#endif // ENGINE_TEST_HPP
