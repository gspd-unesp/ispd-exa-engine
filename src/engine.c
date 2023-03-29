#include <stdio.h>
#include <engine.h>

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
void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event) {
#if SIM == 0 /* ROOT-Sim */
    ScheduleNewEvent(service, time, event_type, event, sizeof(*event));
#endif
}

int main(int argc, char **argv) {
  printf("Engine\n");
  return 0;
}
