#ifndef ENGINE_MACHINE_H
#define ENGINE_MACHINE_H

#include <engine.h>
#include <datatypes/queue.h>

struct machine_metrics {
    double   proc_megaflops;
    double   proc_time;
    unsigned proc_tasks;
};

struct machine {
    sid_t                  id;
    double                 power;
    double                 load_factor;
    unsigned               cores;
    unsigned               used_cores;
    queue(struct task)     task_queue;
    struct machine_metrics metrics;
};

/**
 * @brief It returns the time taken in seconds (s) to process
 *        a computational tasks with the specified processing
 *        size in mega-flops (MFlops) by the specified machine.
 *
 *        Further, note that the higher the machine's load
 *        higher is the time taken to process the computational
 *        task, since higher load implies a lower available
 *        computational power.
 *
 *        In summary, this function is used to reduce the boilerplate
 *        of the following expression.
 *
 *          (processing size) / ((1 - load factor) * computational power)
 *
 *        the result is the time taken in seconds (s).
 *
 * @param m         the machine to process a computational task
 *                  with the respective processing size
 * @param proc_size the computational task's processing size
 *
 * @return the time taken by the specified machine to process the
 *         computational task with the equivalent processing size
 */
double time_to_process(struct machine *m, double proc_size);

/*
 * Event Handlers
 */

/**
 * @brief It processes the arrival of a specified task by the
 *        specified machine at the specified time.
 *
 *        At this time, the machine checks if there is an available
 *        core to process the arrived task. If so, the task will be
 *        immediately attended, otherwise, the task is inserted into
 *        a task queue to wait to be attended.
 *
 * @param m    the machine to process the arrival of the task
 * @param time the time when the machine starts to process the task
 * @param task the task to be processed
 */
void machine_task_arrival(struct machine *m, timestamp_t time, struct task *task);

/**
 * @brief It processes the attendance of a specified task by the
 *        specified machine at the specified time.
 *
 *        At this time, the machine processes the attending task;
 *        this processing updates the machine metrics.
 *
 * @param m    the machine to process the attendance of the task
 * @param time the time when the machine starts to process the task
 * @param task the task to be processed
 */
void machine_task_attendance(struct machine *m, timestamp_t time, struct task *task);

/**
 * @brief It processes the departure of a specified task by the
 *        specified machine at the specified time.
 *
 * @param m    the machine to process the departure of the task
 * @param time the time when the machine starts to process the task
 * @param task the task to be processed
 */
void machine_task_departure(struct machine *m, timestamp_t time, struct task *task);

#endif // ENGINE_MACHINE_H
