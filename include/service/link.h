#ifndef ENGINE_LINK_H
#define ENGINE_LINK_H

#include <datatypes/queue.h>
#include <engine.h>

struct link_metrics
{
    double comm_mbits;
    double comm_time;
    int    comm_tasks;
};

struct link
{
    sid_t                        id;
    sid_t                        from;
    sid_t                        to;
    struct link_metrics          metrics;
    double                       bandwidth;
    double                       latency;
    double                       load_factor;
    timestamp_t                  available_t;
    ENGINE_TEMPORARY timestamp_t lvt;
};

struct link *link_new(double bandwidth, double latency, double load_factor);

void link_task_arrival(struct link *l, timestamp_t time, struct task *t);

#endif // ENGINE_LINK_H
