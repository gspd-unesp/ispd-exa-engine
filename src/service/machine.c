#include <service/machine.h>

ENGINE_INLINE double time_to_process(struct machine *m, double proc_size) {
    return proc_size / ((1.0 - m->load_factor) * m->power);
}

ENGINE_INLINE void machine_task_arrival(struct machine *m, timestamp_t time, struct task *task) {
    if (UNLIKELY(!m))
        die("machine_task_arrival: machine is NULL");
    if (UNLIKELY(!task))
        die("machine_task_arrival: task is NULL");

    struct event e;
    e.task = task;

    /*
     * Since all machine cores is being used by the running
     * tasks, then the current incoming tasks will be inserted
     * into the waiting task queue.
     */
    if (m->used_cores == m->cores) {
        queue_insert(m->waiting_tasks, task);
    } else {
        /*
         * Since there at least one available core in the machine,
         * then the current task will be sent to that core, such
         * that the task will be attended immediately.
         */
        m->used_cores++;

        schedule_event(m->id, time, MACHINE_TASK_ATTENDANCE, &e, sizeof(e));
    }
}

ENGINE_INLINE void machine_task_attendance(struct machine *m, timestamp_t time, struct task *task) {
    if (UNLIKELY(!m))
        die("machine_task_attendance: machine is NULL");
    if (UNLIKELY(!task))
        die("machine_task_attendance: task is NULL");

    const double proc_size = task->proc_size;
    const double proc_time = time_to_process(m, proc_size);

    /* Update machine's metrics */
    m->metrics.proc_megaflops += proc_size;
    m->metrics.proc_time      += proc_time;
    m->metrics.proc_tasks++;

    struct event e;
    e.task = task;

    schedule_event(m->id, time + proc_time, MACHINE_TASK_DEPARTURE, &e, sizeof(e));
}

ENGINE_INLINE void machine_task_departure(struct machine *m, timestamp_t time, struct task *task) {
    if (UNLIKELY(!m))
        die("machine_task_departure: machine is NULL");
    if (UNLIKELY(!task))
        die("machine_task_departure: task is NULL");

    /*
     * Since the waiting task queue is empty, then the
     * current departing task will leave a core vacant.
     * Therefore, the amount of used cores of the machine
     * is decreased.
     */
    if (queue_empty(m->waiting_tasks)) {
        m->used_cores--;
    } else {
        /*
         * Since the waiting task queue is not empty, then
         * the task in front of the queue is removed from it
         * and send it to be attended.
         */
        struct event e;
        e.task = queue_remove(m->waiting_tasks);

        schedule_event(m->id, time, MACHINE_TASK_ATTENDANCE, &e, sizeof(e));
    }
}