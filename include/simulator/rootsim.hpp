#ifndef ENGINE_TIMEWARP_HPP
#define ENGINE_TIMEWARP_HPP

#include <ROOT-Sim.h>
#include <simulator/simulator.hpp>

namespace ispd::sim
{
/// \brief ROOT-Sim Simulator class.
///
/// \details
///        This class implements the simulation engine using the ROOT-Sim,
///        a general purpose mutli-threaded distributed simulator.
class ROOTSimSimulator : public Simulator
{
public:
    /// \brief ROOTSimSimulator ctor.
    ///
    /// \param configuration The simulation configuration.
    explicit ROOTSimSimulator(struct simulation_configuration &&configuration)
        : m_Conf(std::move(configuration))
    {}

    /// \brief It executes the simulation using the Time Warp Optimistic
    ///        Synchronization Protocol, using the ROOT-Sim's implementation.
    void simulate() override;

private:
    /// \brief Simulation Configuration.
    ///
    ///        This structure contains the ROOT-Sim's simulator
    ///        configuration, such that, different options may be
    ///        modified to tune the simulator.
    struct simulation_configuration m_Conf;
};
} // namespace ispd::sim

#endif // ENGINE_TIMEWARP_HPP
