#include <service/machine.h>

ENGINE_INLINE double time_to_process(struct machine *m, double proc_size) {
    return proc_size / ((1.0 - m->load_factor) * m->power);
}

void machine_task_arrival(struct machine *m, timestamp_t time, struct task *task) {
    if (UNLIKELY(!m))
        die("machine_task_arrival: machine is NULL");
    if (UNLIKELY(!task))
        die("machine_task_arrival: task is NULL");

    struct event e;
    e.task = task;

    schedule_event(m->id, time, MACHINE_TASK_ATTENDANCE, &e, sizeof(e));
}

void machine_task_attendance(struct machine *m, timestamp_t time, struct task *task) {
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

void machine_task_departure(struct machine *m, timestamp_t time, struct task *task) {
    if (UNLIKELY(!m))
        die("machine_task_departure: machine is NULL");
    if (UNLIKELY(!task))
        die("machine_task_departure: task is NULL");
}