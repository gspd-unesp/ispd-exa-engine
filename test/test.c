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
        if (me == 0)
        {
            struct machine *m = (struct machine *)s;
            printf("\nMachine Metrics\n");
            printf("  - LVT.............: %lf\n", m->lvt);
            printf("  - Processed MFlops: %lf\n", m->metrics.proc_mflops);
            printf("  - Processed Time..: %lf\n", m->metrics.proc_time);
            printf("  - Processed Tasks.: %d\n", m->metrics.proc_tasks);
        }
        break;
    }
    case LP_INIT: {
        struct machine *m = machine_new(2.0, 0.0, 2);
        SetState(m);
        m->id = me;

        struct event e1;
        e1.task.proc_size = 80;
        e1.task.comm_size = 50;

        struct event e2;
        e2.task.proc_size = 100;
        e2.task.comm_size = 50;

        struct event e3;

        e3.task.proc_size = 60; e3.task.comm_size = 50;

        schedule_event(m->id, 0.0, MACHINE_TASK_ARRIVAL, &e1, sizeof(e1));
        schedule_event(m->id, 1e-52, MACHINE_TASK_ARRIVAL, &e2, sizeof(e2));
        schedule_event(m->id, 75.0, MACHINE_TASK_ARRIVAL, &e3, sizeof(e3));
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
