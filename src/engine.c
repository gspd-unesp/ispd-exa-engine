#include <stdio.h>
#include <engine.h>

ENGINE_INLINE void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event, size_t event_size) {
#if SIM == 0 /* ROOT-Sim */
    ScheduleNewEvent(service, time, event_type, event, event_size);
#endif
}