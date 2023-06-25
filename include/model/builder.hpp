#ifndef ENGINE_MODEL_BUILDER_HPP
#define ENGINE_MODEL_BUILDER_HPP

#include <functional>
#include <scheduler/round_robin.hpp>
#include <scheduler/scheduler.hpp>
#include <service/master.hpp>
#include <simulator/simulator.hpp>

#include <stdexcept>

namespace ispd
{
namespace model
{

/**
 * @brief An enumeration that lists the all available master's schedulers.
 */
enum class MasterScheduler
{
    ROUND_ROBIN
};

/**
 * @brief A builder is a class that provides many utility functions to build
 *        a model to be simulated in an efficient and convenient way, such that
 *        a great amount of boilerplate is reduced to build a model.
 */
class Builder
{
public:
    /**
     * @brief Constructor which specifies a pointer to the simulator
     *        in whic the model to be simulated will be built.
     *
     * @param a pointer to the simulator in which the model will be built
     */
    explicit Builder(ispd::sim::Simulator *const simulator)
        : m_Simulator(simulator)
    {
        if (not simulator)
            throw std::invalid_argument(
                "The specified pointer to the simulator is null");
    }

    /**
     * @brief Registers a service of type master in the model to be simulated
     *        with the specified master identifier and scheduler type.
     *
     *        Further, after the master service initialization, the specified
     *        `callback` function will be called.
     *
     * @param masterId the master's identifier
     * @param schedulerType the master scheduler type
     * @param callback a piece of code that will be called after the master's
     *                 initialization
     */
    void registerMaster(const sid_t                     masterId,
                        MasterScheduler                 schedulerType,
                        std::function<void(Master *)> &&callback);

    /**
     * @brief Registers a service of type machine in the model to be simulated
     *        with the specified machine identfier, power, load factor and
     *        cores.
     *
     * @param machineId the machine's identifier
     * @param power the machine's power in megaflops/s
     * @param loadFactor the machine's load factor
     * @param cores the machine's amount of cores
     */
    void registerMachine(const sid_t  machineId,
                         const double power,
                         const double loadFactor,
                         const int    cores);

    /**
     * @brief Registers a service of type link in the model to be simulated
     *        with the specified link identifier, the link's source identifier,
     *        the link's destination identifier, the bandwidth, the load factor
     *        and the latency of the link.
     *
     * @param linkId the link's identifier
     * @param from the link's source identifier
     * @param to the link's destination identifier
     * @param bandwdith the bandwidth in megabits/s
     * @param loadFactor the load factor
     * @param latency the latency in seconds
     */
    void registerLink(const sid_t  linkId,
                      const sid_t  from,
                      const sid_t  to,
                      const double bandwidth,
                      const double loadFactor,
                      const double latency);

    /**
     * Registers a service of the type switch in the model to be simulated with the specifed
     * id, bandiwidth (megabits), load factor and latency.
     * @param switchId switch's identifier.
     * @param bandwidth switch's bandiwidth
     * @param loadFactor switch's loadFactor
     * @param latency   switch's latency
     */
    void registerSwitch ( const sid_t switchId,
                          const double bandwidth,
                          const double loadFactor,
                          const double latency);



    void registerDummy(const sid_t dummyId);

private:
    /**
     * @brief A pointer to the siulator in which the model
     *        to be simulated will be built.
     */
    ispd::sim::Simulator *const m_Simulator;
};

namespace workload
{

namespace zeroth
{

/**
 * @brief Adds a constant size workload to the specified master.
 *
 *        Further, the workload generated to the master is constant-sized,
 *        that is, the processing and communication sizes are equal for all
 *        tasks that will be generated.
 *
 *        Moreover, if the `jittered` is false, then all tasks will arrive
 *        in the master at timestamp `0.0`, otherwise, the i-th task will arrive
 *        at the timestamp of the previous task plus `1e-52`. This is done to
 *        imitates that all tasks is still arriving at `0.0` but ensuring the
 *        total ordering of events with relation in its generation order.
 *
 * @param masterId the master identifier which represents the master that
 *                 will receive these tasks
 * @param processingSize the tasks' processing size in megaflops
 * @param communicationSize the tasks' communication size in megabits
 * @param taskAmount the amount of tasks that will be generated
 * @param jittered it indicates if the events carrying those tasks will be
 *                 jittered or not.
 */
void addConstantSizedWorkload(const sid_t    masterId,
                              const double   processingSize,
                              const double   communicationSize,
                              const uint32_t taskAmount,
                              const bool     jittered = false);

} // namespace zeroth

namespace exp
{
void addConstantSizedWorkload(const sid_t    masterId,
                              const double   processingSize,
                              const double   communicationSize,
                              const uint32_t taskAmount);

} // namespace exp

} // namespace workload

} // namespace model
} // namespace ispd
#endif // ENGINE_MODEL_BUILDER_HPP
