#ifndef ENGINE_MODEL_BUILDER_HPP
#define ENGINE_MODEL_BUILDER_HPP

#include <simulator/simulator.hpp>

namespace ispd
{
namespace model
{
class Builder
{
public:
    explicit Builder(Simulator *const simulator) noexcept
        : m_Simulator(simulator)
    {}

    void registerMachine(const sid_t    serviceId,
                         const double   power,
                         const double   loadFactor,
                         const int cores);

private:
    Simulator *const m_Simulator;
};
} // namespace model
} // namespace ispd
#endif // ENGINE_MODEL_BUILDER_HPP
