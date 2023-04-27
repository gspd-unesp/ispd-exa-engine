#include <engine.h>
#include <service/link.h>
#include <service/machine.h>
#include <stdio.h>

#ifndef NUM_LPS
#    define NUM_LPS 1
#endif

#ifndef NUM_TASKS
#    define NUM_TASKS 50
#endif

#ifndef NUM_THREADS
#    define NUM_THREADS 0
#endif

void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s)
{
    switch (event_type)
    {
    case LP_FINI: {
        break;
    }
    case LP_INIT: {
        break;
    }
    case MACHINE_TASK_ARRIVAL: {
        struct event *e = (struct event *)content;
        machine_task_arrival((struct machine *)s, now, &e->task);
        break;
    }
    case LINK_TASK_ARRIVAL: {
        struct event *e = (struct event *)content;
        link_task_arrival((struct link *)s, now, &e->task);
        break;
    }
    default:
        fprintf(stderr, "Unknown event type\n");
        abort();
    }
}

bool CanEnd(lp_id_t me, const void *snapshot)
{
    return false;
}

struct simulation_configuration conf = {
    .lps              = NUM_LPS,
    .n_threads        = NUM_THREADS,
    .termination_time = 0,
    .gvt_period       = 1000,
    .log_level        = LOG_INFO,
    .stats_file       = "phold",
    .ckpt_interval    = 0,
    .prng_seed        = 0,
    .core_binding     = false,
    .serial           = false,
    .dispatcher       = ProcessEvent,
    .committed        = CanEnd,
};

int main(int argc, char **argv)
{
    --argc, ++argv;

    RootsimInit(&conf);
    return RootsimRun();
}
