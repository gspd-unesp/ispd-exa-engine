#include <service/link.h>

ENGINE_INLINE double time_to_transmit(struct link *l, double comm_size) {
    return l->latency + comm_size / ((1.0 - l->load_factor) * l->bandwidth);
}

ENGINE_INLINE void link_task_arrival(struct link *l, timestamp_t time, struct task *task) {
    if (UNLIKELY(!l))
        die("link_task_arrival: link is NULL");
    if (UNLIKELY(!task))
        die("link_task_arrival: task is NULL");

    struct event e;
    e.task = task;

    schedule_event(l->id, time, LINK_TASK_ATTENDANCE, &e, sizeof(e));
}

ENGINE_INLINE void link_task_attendance(struct link *l, timestamp_t time, struct task *task) {
    if (UNLIKELY(!l))
        die("link_task_attendance: link is NULL");
    if (UNLIKELY(!task))
        die("link_task_attendance: task is NULL");

    const double comm_size = task->comm_size;
    const double comm_time = time_to_transmit(l, comm_size);

    /* Update link's metrics */
    l->metrics.transmitted_megabits += comm_size;
    l->metrics.transmitted_time     += comm_time;
    l->metrics.transmitted_packets++;

    struct event e;
    e.task = task;

    schedule_event(l->id, time + comm_time, LINK_TASK_DEPARTURE, &e, sizeof(e));
}

ENGINE_INLINE void link_task_departure(struct link *l, timestamp_t time, struct task *task) {
    if (UNLIKELY(!l))
        die("link_task_departure: link is NULL");
    if (UNLIKELY(!task))
        die("link_task_departure: task is NULL");

    struct event next_dest_ev;
    next_dest_ev.task = task;

    /// [!!!]: Not always the event type must be a MACHINE_TASK_ARRIVAL. Since the next destination may be a master or
    ///        a cluster, not necessarily a machine.
    schedule_event(l->to, time, MACHINE_TASK_ARRIVAL, &next_dest_ev, sizeof(next_dest_ev));
}