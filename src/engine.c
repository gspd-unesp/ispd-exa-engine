#include <stdio.h>
#include <engine.h>

ENGINE_INLINE void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event) {
#if SIM == 0 /* ROOT-Sim */
    ScheduleNewEvent(service, time, event_type, event, sizeof(*event));
#endif
}

int main(int argc, char **argv) {
  printf("Engine\n");
  return 0;
}
