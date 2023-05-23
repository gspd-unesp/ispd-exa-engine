#ifndef ENGINE_MODEL_BUILDER_HPP
#define ENGINE_MODEL_BUILDER_HPP

#include <functional>
#include <scheduler/round_robin.hpp>
#include <scheduler/scheduler.hpp>
#include <service/master.hpp>
#include <simulator/simulator.hpp>

namespace ispd
{
namespace model
{

enum class MasterScheduler
{
    ROUND_ROBIN
};

class Builder
{
public:
    explicit Builder(Simulator *const simulator) noexcept
        : m_Simulator(simulator)
    {}

    void registerMaster(const sid_t                     masterId,
                        MasterScheduler                 schedulerType,
                        std::function<void(Master *)> &&callback);

    void registerMachine(const sid_t  machineId,
                         const double power,
                         const double loadFactor,
                         const int    cores);

    void registerLink(const sid_t  linkId,
                      const sid_t  from,
                      const sid_t  to,
                      const double bandwidth,
                      const double loadFactor,
                      const double latency);

private:
    Simulator *const m_Simulator;
};

namespace workload
{

namespace zeroth
{
void addConstantSizedWorkload(const sid_t    masterId,
                              const double   processingSize,
                              const double   communicationSize,
                              const uint32_t taskAmount,
                              const bool     jittered = false);

}
} // namespace workload

} // namespace model
} // namespace ispd
#endif // ENGINE_MODEL_BUILDER_HPP
