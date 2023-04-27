#include <service/machine.h>
#include <string.h>

/**
 * @brief It calculates the time taken in seconds by the machine to process
 *        a customer with the specified processing size in mega-flops.
 *
 * @details
 *        It is the caller's responsibility to ensure that the specified
 *        machine is a non-NULL pointer. Otherwise, unexpected behavior
 *        may occur.
 *
 *        Further, it is also the caller's responsibility to ensure that
 *        the processing size is a non-negative real number.
 *
 * @param m the machine
 * @param proc_size the processing size
 *
 * @return the time taken in seconds by the machine to process a
 *         customer with the specified processing size in mega-flops
 */
ENGINE_INLINE static double time_to_process(const struct machine *m, double proc_size)
{
    return proc_size / ((1.0 - m->load_factor) * m->power_per_proc);
}

/**
 * @brief It calculates the time necessary by a task to be attended in the
 *        specified machine. In other words, it is calculated the waiting
 *        time of a task to be processed by one of the machine's core.
 *
 *        Moreover, the core of the machine to process the task is set
 *        in the placeholder.
 *
 * @param m the machine
 * @param core the placeholder in which will be put the index of the
 *             core that will process the task. The core index is in
 *             the interval [0, cores - 1]; in which `cores` is the
 *             total amount of cores in the machine.
 *
 * @return the waiting time for a task to be attended in the specified
 *         machine
 */
ENGINE_INLINE static double time_to_attend(const struct machine *m, int *core)
{
    timestamp_t least_core_time = DBL_MAX;
    int         core_index;

    for (int i = 0; i < m->cores; i++)
    {
        if (least_core_time > m->core_free_t[i])
        {
            least_core_time = m->core_free_t[i];
            core_index      = i;
        }
    }

    *core = core_index;
    return least_core_time;
}

struct machine *machine_new(double power, double load_factor, int cores)
{
    struct machine *m = rs_malloc(sizeof(struct machine));

    memset(m, 0, sizeof(struct machine));

    m->power          = power;
    m->power_per_proc = power / cores;
    m->load_factor    = load_factor;
    m->cores          = cores;
    m->core_free_t    = rs_calloc(m->cores, sizeof(timestamp_t));

    return m;
}

void machine_task_arrival(struct machine *m, timestamp_t time, struct task *t)
{
    const double proc_size = t->proc_size;
    const double proc_time = time_to_process(m, proc_size);

    m->metrics.proc_mflops += proc_size;
    m->metrics.proc_time   += proc_time;
    m->metrics.proc_tasks++;

    int               core_index;
    const timestamp_t least_core_time = time_to_attend(m, &core_index);
    const timestamp_t waiting_time    = time < least_core_time ? least_core_time - time : 0.0;
    const timestamp_t departure_time  = time + waiting_time + proc_time;

    m->core_free_t[core_index] = departure_time;
    m->lvt                     = departure_time;

    DEBUG("Task: (%lf, %lf), Waiting Time: %lf, Departure Time: %lf\n", t->proc_size, t->comm_size, waiting_time,
          departure_time);
}