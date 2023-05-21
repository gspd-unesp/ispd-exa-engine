#ifndef ENGINE_MACHINE_HPP
#define ENGINE_MACHINE_HPP

#include <core/core.hpp>
#include <limits>
#include <service/service.hpp>
#include <allocator/rootsim_allocator.hpp>

struct MachineMetrics
{
    double m_ProcMFlops;
    double m_ProcTime;
    int    m_ProcTasks;
};

class Machine : public Service
{
public:
    /**
     * @brief Constructor which specifies the machine's identifier, power in megaflops,
     *        the load factor, and the amount of cores.
     *
     * @details
     *        It is the caller's responsibility to ensure that
     *        the specified service's identifier is unique.
     *
     *        Otherwise, unexpected behavior may occur.
     *
     * @param id the machine's identifier
     * @param power the machine's power in megaflops
     * @param loadFactor the load factor (a value in the interval [0, 1])
     * @param cores the amount of cores
     */
    explicit Machine(const sid_t id, const double power, const double loadFactor, const int cores)
        : Service(id), m_PowerPerProc(power / cores), m_LoadFactor(loadFactor), m_Cores(cores),
          m_CoreFreeTimes(ROOTSimAllocator<timestamp_t>::allocate<timestamp_t>(cores))
    {}

    /**
     * @brief It calculates the time taken in seconds by the machine to process
     *        a customer with the specified processing size in megaflops.
     *
     * @details
     *        It is the caller's responsibility to ensure that
     *        the processing size is a non-negative real number.
     *
     * @param proc_size the processing size
     *
     * @return the time taken in seconds by the machine to process a
     *         customer with the specified processing size in megaflops
     */
    ENGINE_INLINE double timeToProcess(const double procSize) const
    {
        return procSize / ((1.0 - m_LoadFactor) * m_PowerPerProc);
    }

    /**
     * @brief It calculates the time necessary by a task to be attended in the
     *        specified machine. In other words, it is calculated the waiting
     *        time of a task to be processed by one of the machine's core.
     *
     *        Moreover, the core of the machine to process the task is set
     *        in the placeholder.
     *
     * @param core the placeholder in which will be put the index of the
     *             core that will process the task. The core index is in
     *             the interval [0, cores - 1]; in which `cores` is the
     *             total amount of cores in the machine.
     *
     * @return the waiting time for a task to be attended in the specified
     *         machine
     */
    ENGINE_INLINE double timeToAttend(int *core) const
    {
        timestamp_t leastCoreTime = std::numeric_limits<timestamp_t>::max();
        int         coreIndex;

        for (int i = 0; i < m_Cores; i++) {
            if (leastCoreTime > m_CoreFreeTimes[i]) {
                leastCoreTime = m_CoreFreeTimes[i];
                coreIndex     = i;
            }
        }

        *core = coreIndex;
        return leastCoreTime;
    }

    /**
     * @brief It processes the arrival of a task.
     *
     * @param time the time in which the task has arrived
     * @param t the arrived task
     */
    void onTaskArrival(timestamp_t time, const Event *event) override;

    /**
     * @brief It returns a const (read-only) reference to the machine metrics.
     *
     * @return a const (read-only) reference to the machine metrics
     */
    const MachineMetrics &getMetrics() const
    {
        return m_Metrics;
    }

    ENGINE_TEMPORARY timestamp_t getLocalVirtualTime() const
    {
        return m_LVT;
    }

private:
    MachineMetrics               m_Metrics{};
    double                       m_PowerPerProc;
    double                       m_LoadFactor;
    int                          m_Cores;
    timestamp_t                 *m_CoreFreeTimes;
    ENGINE_TEMPORARY timestamp_t m_LVT;
};

#endif // ENGINE_MACHINE_HPP
