#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <core/core.hpp>

#define TASK_ARRIVAL 1

/**
 * Simulator
 *
 *  0 - ROOT-Sim
 */
#define SIM 0

#if SIM == 0
#    define ROOTSIM_ENGINE
extern "C"
{
#    include <ROOT-Sim.h>

    typedef lp_id_t   sid_t;
    typedef simtime_t timestamp_t;
}

#endif // ROOT-Sim

ENGINE_INLINE void schedule_event(const sid_t id, const timestamp_t time, const unsigned eventType, const void *event,
                                  const std::size_t eventSize)
{
#if SIM == 0
    ScheduleNewEvent(id, time, eventType, event, eventSize);
#endif // ROOT-Sim
}

#endif // ENGINE_HPP
