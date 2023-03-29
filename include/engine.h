#ifndef ENGINE_H
#define ENGINE_H

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

/**
 * @brief It schedules a event to be processed by the
 *        specified service at that time.
 *
 *        Further, an event type is specified to indicate
 *        how the event will be processed.
 *
 * @param service    the service
 * @param time       the time
 * @param event_type the event type
 * @param event      the event
 */
void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event);

#endif // ENGINE_H
