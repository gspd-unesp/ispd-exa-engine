#ifndef ENGINE_MACHINE_H
#define ENGINE_MACHINE_H

#include <datatypes/queue.h>
#include <engine.h>

struct machine_metrics
{
    double proc_mflops;
    double proc_time;
    int    proc_tasks;
};

struct machine
{
    sid_t                        id;
    struct machine_metrics       metrics;
    double                       power;
    double                       load_factor;
    int                          cores;
    timestamp_t                 *core_free_t;
    ENGINE_TEMPORARY timestamp_t lvt;
};

struct machine *machine_new(double power, double load_factor, int cores);

void machine_task_arrival(struct machine *m, timestamp_t time, struct task *t);

#endif // ENGINE_MACHINE_H
