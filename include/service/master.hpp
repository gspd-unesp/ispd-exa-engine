#ifndef ENGINE_MASTER_HPP
#define ENGINE_MASTER_HPP

#include <vector>
#include <service/service.hpp>
#include <scheduler/scheduler.hpp>

class Master : public Service
{
public:
    explicit Master(const sid_t id, Scheduler<sid_t> *scheduler)
        : Service(id), m_Scheduler(scheduler) {}

    /**
     * @brief It schedules the specified task to a scheduled slave.
     *
     *        The slave is scheduled using some scheduling policy
     *        that has been previously configured to this master.
     *
     * @param time the time in which this task has arrived
     * @param t the task to be sent to the scheduled slave
     */
    void onTaskSchedule(timestamp_t time, const Task *t);

    /**
     * Returns the scheduler.
     *
     * @return the scheduler
     */
     Scheduler<sid_t> *getScheduler() { return m_Scheduler; }
private:
    Scheduler<sid_t> *m_Scheduler;
};

#endif // ENGINE_MASTER_HPP