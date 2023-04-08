#include <stdio.h>
#include <string.h>
#include <engine.h>
#include <mm/mm.h>
#include <service/machine.h>
#include <service/link.h>
#include <time.h>
#include <pthread.h>

#ifndef NUM_LPS
#define NUM_LPS 50
#endif

#ifndef NUM_TASKS
#define NUM_TASKS 100000
#endif

#ifndef NUM_THREADS
#define NUM_THREADS 0
#endif

static struct machine machines[NUM_LPS];

pthread_mutex_t print_lock;
FILE *output;

void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s) {
    switch(event_type) {
        case LP_FINI:
            pthread_mutex_lock(&print_lock);
            if (me < (NUM_LPS - 1)) {
                struct machine *m = &machines[me];
                fprintf(output, "Machine Metrics (%lu)\n", m->id);
                fprintf(output, "  - Current Time...............: %lf\n", now);
                fprintf(output, "  - Machine Processed Megaflops: %lf\n", m->metrics.proc_megaflops);
                fprintf(output, "  - Machine Processed Time.....: %lf\n", m->metrics.proc_time);
                fprintf(output, "  - Machine Processed Tasks....: %u\n", m->metrics.proc_tasks);
                fprintf(output, "  - Machine Task Queue Size....: %zu\n", queue_size(m->waiting_tasks));
            } else {
//                fprintf(output, "Link Metrics\n");
//                fprintf(output, "  - Current Time...............: %lf\n", now);
//                fprintf(output, "  - Link Transmitted Megabits..: %lf\n", l->metrics.transmitted_megabits);
//                fprintf(output, "  - Link Transmitted Time......: %lf\n", l->metrics.transmitted_time);
//                fprintf(output, "  - Link Transmitted Packets...: %u\n", l->metrics.transmitted_packets);
//                fprintf(output, "  - Link Task Queue Size.......: %zu\n", queue_size(l->waiting_tasks));
            }
            pthread_mutex_unlock(&print_lock);
            break;
        case LP_INIT: {
            if (me < (NUM_LPS - 1)) {
                struct machine *m = &machines[me];
                memset(m, 0, sizeof(struct machine));

                m->id = me;
                m->power = 500;
                m->load_factor = 0.2;
                m->cores = 8;
                queue_init(m->waiting_tasks);

                SetState(m);
            } else { // lps: [0, 7], machines: [0, 6].
                for (int i = 0; i < NUM_TASKS; i++) {
                    struct event e;
                    e.task.proc_size = 10000;
                    e.task.comm_size = 4000;

                    schedule_event(rand() % (NUM_LPS - 1), 0, MACHINE_TASK_ARRIVAL, &e, sizeof(e));
                }
            }
            break;
        }
        case MACHINE_TASK_ARRIVAL:
            machine_task_arrival((struct machine *)s, now, &((struct event *)content)->task);
            break;
        case MACHINE_TASK_ATTENDANCE:
            machine_task_attendance((struct machine *)s, now, &((struct event *)content)->task);
            break;
        case MACHINE_TASK_DEPARTURE:
            machine_task_departure((struct machine *)s, now, &((struct event *)content)->task);
            break;
        case LINK_TASK_ARRIVAL:
            link_task_arrival((struct link *)s, now, &((struct event *)content)->task);
            break;
        case LINK_TASK_ATTENDANCE:
            link_task_attendance((struct link *)s, now, &((struct event *)content)->task);
            break;
        case LINK_TASK_DEPARTURE:
            link_task_departure((struct link *)s, now, &((struct event *)content)->task);
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


int main(int argc, char **argv) {
    --argc, ++argv;

    if (0 == argc) {
        printf("Output file must be specified.\n");
        abort();
    }

    pthread_mutex_init(&print_lock, NULL);
    memset(machines, 0, sizeof(struct machine) * NUM_LPS);

    char filepath[64];
    memset(filepath, '\0', 64);
    strcat(filepath, argv[0]);
    strcat(filepath, conf.serial ? "output_serial" : "output_parallel");

    output = fopen(filepath, "w");

    if (!output) {
        printf("Output file located at %s could not be opened\n", filepath);
        abort();
    }

    fprintf(output, "%ld\n", time(NULL));

    srand(101010);
    RootsimInit(&conf);
    return RootsimRun();
}
