#ifndef ENGINE_TIMEWARP_HPP
#define ENGINE_TIMEWARP_HPP

#include <simulator/simulator.hpp>

/**
 * @brief Time Warp Simulator class.
 */
class TimeWarpSimulator : public Simulator
{
public:
    /**
     * @brief It executes the simulation using the
     *        Time Warp Optimistic Synchronization Protocol
     */
    void simulate() override;
};

#endif // ENGINE_TIMEWARP_HPP
