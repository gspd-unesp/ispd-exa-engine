#include <stdio.h>
#include <stdarg.h>
#include <engine.h>

ENGINE_INLINE void schedule_event(sid_t service, timestamp_t time, unsigned event_type, void *event) {
#if SIM == 0 /* ROOT-Sim */
    ScheduleNewEvent(service, time, event_type, event, sizeof(*event));
#endif
}

void die(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    fputc('\n', stderr);
    abort();
}

int main(int argc, char **argv) {
  printf("Engine\n");
  return 0;
}
