#include <stdio.h>
#include <engine.h>
#include <service/machine.h>
#include <service/link.h>

#ifndef NUM_LPS
#define NUM_LPS 1000
#endif

#ifndef NUM_TASKS
#define NUM_TASKS 50
#endif

#ifndef NUM_THREADS
#define NUM_THREADS 0
#endif

#define EQUAL_TASKS 0

void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s)
{
    switch (event_type)
    {
        case LP_FINI:
        {
            if (me == 0)
            {
                struct machine *m = (struct machine *) s;
                printf("\nMachine Metrics\n");
                printf("  - LVT.............: %lf\n", m->lvt);
                printf("  - Processed MFlops: %lf\n", m->metrics.proc_mflops);
                printf("  - Processed Time..: %lf\n", m->metrics.proc_time);
                printf("  - Processed Tasks.: %d\n", m->metrics.proc_tasks);
            } else if (me == 1)
            {
                struct link *l = (struct link *) s;
                printf("\nLink Metrics\n");
                printf("  - LVT,,,,,,,,,,,,,,,: %lf\n", l->lvt);
                printf("  - Communicated Mbits: %lf\n", l->metrics.comm_mbits);
                printf("  - Communicated Time.: %lf\n", l->metrics.comm_time);
                printf("  - Communicated Tasks: %d\n", l->metrics.comm_tasks);
            }
            break;
        }
        case LP_INIT:
            if (me == 0)
            {
                struct machine *m = machine_new(5000.0, 0.2, 8);
                m->id = me;

                SetState(m);
            } else
            {
                struct link *l = link_new(1000.0, 5, 0.2);
                l->id = me;

                SetState(l);

                timestamp_t jitter = 0;
                for (int i = 0; i < NUM_TASKS; i++)
                {
                    struct event e;
#if EQUAL_TASKS == 1
                    e.task.proc_size = 1000;
                    e.task.comm_size = 500;
#else
                    e.task.proc_size = 1000 + 100 * i;
                    e.task.comm_size = 500 + 200 * i;
#endif // EQUAL_TASKS
                    e.task.tid = i;
                    schedule_event(me, jitter, LINK_TASK_ARRIVAL, &e, sizeof(e));
                    jitter += 1e-12;
                }
            }

            break;
        case MACHINE_TASK_ARRIVAL:
        {
            struct event *e = (struct event *) content;
            machine_task_arrival((struct machine *) s, now, &e->task);
            break;
        }
        case LINK_TASK_ARRIVAL:
        {
            struct event *e = (struct event *) content;
            link_task_arrival((struct link *) s, now, &e->task);
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
        .lps = NUM_LPS,
        .n_threads = NUM_THREADS,
        .termination_time = 0,
        .gvt_period = 1000,
        .log_level = LOG_INFO,
        .stats_file = "phold",
        .ckpt_interval = 0,
        .prng_seed = 0,
        .core_binding = false,
        .serial = false,
        .dispatcher = ProcessEvent,
        .committed = CanEnd,
};

int main(int argc, char **argv)
{
    --argc, ++argv;

    RootsimInit(&conf);
    return RootsimRun();
}
