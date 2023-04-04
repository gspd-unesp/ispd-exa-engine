#ifndef ENGINE_LINK_H
#define ENGINE_LINK_H

#include <engine.h>
#include <stdbool.h>
#include <datatypes/queue.h>

struct link_metrics {
    double   transmitted_megabits;
    double   transmitted_time;
    unsigned transmitted_packets;
};

struct link {
    sid_t               id;
    sid_t               from;
    sid_t               to;
    double              bandwidth;
    double              latency;
    double              load_factor;
    queue(struct task)  waiting_tasks;
    bool                available_link;
    struct link_metrics metrics;
};

/**
 * @brief It returns the time taken in seconds (s) to transmit
 *        a computational task with the specified communication
 *        size in megabits (Mb) by the specified link.
 *
 *        Further, note that the higher the link's load higher
 *        is the time taken to transmit the computational task,
 *        since higher load implies a lower available bandwidth.
 *
 *        In summary, this function is used to reduce the boilerplate
 *        of the following expression.
 *
 *          (communication size) / ((1 - load factor) * bandwidth)
 *
 *        the result is the time taken in seconds (s).
 *
 * @param l         the link to transmit the computational task
 *                  with the respective communication size
 * @param comm_size the computational task's communication size
 *
 * @return the time taken by the specified link to transmit the
 *         specified computational task with the equivalent
 *         communication size
 */
double time_to_transmit(struct link *l, double comm_size);

/*
 * Event Handlers
 */

/**
 * @brief It processes the arrival of a specified task by the
 *        specified link at the specified time.
 *
 * @param l    the link to process the arrival of the task
 * @param time the time when the link starts to process the task
 * @param task the task to be processed
 */
void link_task_arrival(struct link *l, timestamp_t time, struct task *task);

/**
 * @brief It processes the attendance of a specified task by the
 *        specified link at the specified time.
 *
 * @param l    the link to process the attending of a task
 * @param time the time when the link starts to process the task
 * @param task the task to be processed
 */
void link_task_attendance(struct link *l, timestamp_t time, struct task *task);

/**
 * @brief It processes the departure of a specified task by the
 *        specified link at the specified time.
 *
 * @param l    the link to process the departure of a task
 * @param time the time when the link starts to process the task
 * @param task the task to be processed
 */
void link_task_departure(struct link *l, timestamp_t time, struct task *task);

#endif // ENGINE_LINK_H
