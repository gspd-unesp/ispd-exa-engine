#ifndef ENGINE_H
#define ENGINE_H

#include <core/core.h>

///
/// Backing Simulator
///     0 - ROOT-Sim
///
#define SIM 0

#if SIM == 0

///
/// ROOT-Sim
///
#include <ROOT-Sim.h>
typedef lp_id_t   sid_t;
typedef simtime_t timestamp_t;

#endif

struct task {
    /*
     * Task's processing size in mega-flops (MFlops).
     */
    double proc_size;

    /*
     * Task's communication size in megabits (Mb).
     */
    double comm_size;
};

struct event {
    struct task *task;
};

/**
 * @brief It schedules a event to be processed by the
 *        specified service at that time.
 *
 *        An event type is specified to indicate which
 *        event handler should process this event.
 *
 *        Further, in addition to the event, the event
 *        size should be specified.
 *
 * @param service    the service
 * @param time       the time
 * @param event_type the event type
 * @param event      the event
 * @param event_size the event size
 */
void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event, size_t event_size);

#endif // ENGINE_H
