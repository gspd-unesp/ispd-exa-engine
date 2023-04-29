#ifndef ENGINE_MASTER_HPP
#define ENGINE_MASTER_HPP

#include <scheduler/scheduler.hpp>
#include <service/service.hpp>
#include <vector>

class Master : public Service
{
public:
    explicit Master(const sid_t id, Scheduler<sid_t> *scheduler) : Service(id), m_Scheduler(scheduler)
    {}

    /**
     * @brief It schedules the specified task to a scheduled slave.
     *
     *        The slave is scheduled using some scheduling policy
     *        that has been previously configured to this master.
     *
     * @param time the time in which this task has arrived
     * @param t the task to be sent to the scheduled slave
     */
    void onTaskArrival(timestamp_t time, const Task *t) override;

    /**
     * Returns the scheduler.
     *
     * @return the scheduler
     */
    Scheduler<sid_t> *getScheduler()
    {
        return m_Scheduler;
    }

private:
    Scheduler<sid_t> *m_Scheduler;
};

#endif // ENGINE_MASTER_HPP
