#include <stdio.h>
#include <string.h>
#include <engine.h>
#include <mm/mm.h>
#include <service/machine.h>
#include <service/link.h>

#ifndef NUM_LPS
#define NUM_LPS 2
#endif

#ifndef NUM_TASKS
#define NUM_TASKS 100
#endif

#ifndef NUM_THREADS
#define NUM_THREADS 0
#endif

static struct machine *m;
static struct link *l;

int total = 0;

void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s) {
    total++;
    switch(event_type) {
        case LP_FINI:
            printf("Total: %d\n", total);
            if (0 == me) {
                printf("Machine Metrics\n");
                printf("  - Current Time...............: %lf\n", now);
                printf("  - Machine Processed Megaflops: %lf\n", m->metrics.proc_megaflops);
                printf("  - Machine Processed Time.....: %lf\n", m->metrics.proc_time);
                printf("  - Machine Processed Tasks....: %u\n", m->metrics.proc_tasks);
                printf("  - Machine Task Queue Size....: %zu\n", queue_size(m->waiting_tasks));
            } else if (1 == me) {
                printf("Link Metrics\n");
                printf("  - Current Time...............: %lf\n", now);
                printf("  - Link Transmitted Megabits..: %lf\n", l->metrics.transmitted_megabits);
                printf("  - Link Transmitted Time......: %lf\n", l->metrics.transmitted_time);
                printf("  - Link Transmitted Packets...: %u\n", l->metrics.transmitted_packets);
                printf("  - Link Task Queue Size.......: %zu\n", queue_size(l->waiting_tasks));
            }

            break;
        case LP_INIT: {
            if (0 == me) {
                m = mm_malloc(sizeof(struct machine));
                memset(m, 0, sizeof(struct machine));
                m->id = me;
                m->power = 100;
                m->cores = 8;
                m->used_cores = 0;
                m->load_factor = 0.2;
                m->metrics.proc_megaflops = 0;
                m->metrics.proc_time = 0;
                m->metrics.proc_tasks = 0;
                queue_init(m->waiting_tasks);
                SetState(m);
            } else if (1 == me) {
                l = mm_malloc(sizeof(struct link));
                memset(l, 0, sizeof(struct link));
                l->id = me;
                l->from = -1;
                l->to = 0 /* machine */;
                l->bandwidth = 500;
                l->latency = 5;
                l->load_factor = 0.2;
                l->metrics.transmitted_megabits = 0;
                l->metrics.transmitted_time = 0;
                l->metrics.transmitted_packets = 0;
                queue_init(l->waiting_tasks);
                l->available_link = true;
                SetState(l);

                for (int i = 0; i < NUM_TASKS; i++) {
                    struct task *t = mm_malloc(sizeof(struct task));
                    t->proc_size = 50000;
                    t->comm_size = 1000;

                    struct event e;
                    e.task = t;

                    schedule_event(l->id, 0, LINK_TASK_ARRIVAL, &e, sizeof(e));
                }

            } else {
                die("Unknown me\n");
            }
            break;
        }
        case MACHINE_TASK_ARRIVAL:
            machine_task_arrival((struct machine *)s, now, ((struct event *)content)->task);
            break;
        case MACHINE_TASK_ATTENDANCE:
            machine_task_attendance((struct machine *)s, now, ((struct event *)content)->task);
            break;
        case MACHINE_TASK_DEPARTURE:
            machine_task_departure((struct machine *)s, now, ((struct event *)content)->task);
            break;
        case LINK_TASK_ARRIVAL:
            link_task_arrival((struct link *)s, now, ((struct event *)content)->task);
            break;
        case LINK_TASK_ATTENDANCE:
            link_task_attendance((struct link *)s, now, ((struct event *)content)->task);
            break;
        case LINK_TASK_DEPARTURE:
            link_task_departure((struct link *)s, now, ((struct event *)content)->task);
            break;
        default:
            fprintf(stderr, "Unknown event type\n");
            abort();
    }
}

bool CanEnd(lp_id_t me, const void *snapshot) {
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
        .core_binding = true,
        .serial = false,
        .dispatcher = ProcessEvent,
        .committed = CanEnd,
};

int main(void) {
    RootsimInit(&conf);
    return RootsimRun();
}
