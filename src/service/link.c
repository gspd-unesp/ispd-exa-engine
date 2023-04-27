#include <service/link.h>
#include <string.h>

/**
 * @brief It calculates the time taken in seconds by the link to communicate
 *        a customer with the specified communication size in megabits.
 *
 * @details
 *        It is the caller's responsibility to ensure that the specified link
 *        is a non-NULL pointer. Otherwise, unexpected behavior may occur.
 *
 *        Further, it is also the caller's responsibility to ensure that the
 *        communication size is a non-negative real number.
 *
 * @param l         the link
 * @param comm_size the communication size (in megabits)
 *
 * @return the time taken in seconds by the link to process a customer with the
 *         specified communication size in megabits.
 */
ENGINE_INLINE static double time_to_comm(const struct link *l, double comm_size)
{
    return l->latency + (comm_size / ((1.0 - l->load_factor) * l->bandwidth));
}

struct link *link_new(double bandwidth, double latency, double load_factor)
{
    struct link *l = rs_malloc(sizeof(struct link));

    memset(l, 0, sizeof(struct link));

    l->bandwidth   = bandwidth;
    l->latency     = latency;
    l->load_factor = load_factor;
    l->available_t = 0.0;

    return l;
}

void link_task_arrival(struct link *l, timestamp_t time, struct task *t)
{
    const double comm_size = t->comm_size;
    const double comm_time = time_to_comm(l, comm_size);

    const timestamp_t departure_time = l->available_t + comm_time;
    l->available_t                   = departure_time;

    l->metrics.comm_mbits += comm_size;
    l->metrics.comm_time  += comm_time;
    l->metrics.comm_tasks++;

    struct event e;
    e.task = *t;

    l->lvt = departure_time;

    schedule_event(l->to, departure_time, MACHINE_TASK_ARRIVAL, &e, sizeof(e));
}
