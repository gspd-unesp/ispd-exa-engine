#ifndef ENGINE_HPP
#define ENGINE_HPP

#define MACHINE_TASK_ARRIVAL 1
#define MASTER_TASK_SCHEDULE 2

/**
 * Simulator
 *
 *  0 - ROOT-Sim
 */
#define SIM 0

#if SIM == 0
extern "C"
{
#    include <ROOT-Sim.h>

    typedef lp_id_t   sid_t;
    typedef simtime_t timestamp_t;
}

#endif // ROOT-Sim

inline void schedule_event(const sid_t id, const timestamp_t time, const unsigned eventType, const void *event,
                           const std::size_t eventSize)
{
#if SIM == 0
    ScheduleNewEvent(id, time, eventType, event, eventSize);
#endif // ROOT-Sim
}

class Task
{
public:
    explicit Task(const double procSize, const double commSize) : m_ProcSize(procSize), m_CommSize(commSize)
    {}

    double getProcessingSize() const
    {
        return m_ProcSize;
    }
    double getCommunicationSize() const
    {
        return m_CommSize;
    }

private:
    double m_ProcSize;
    double m_CommSize;
};

struct Event
{
public:
    explicit Event(Task task) : m_Task(task)
    {}
    const Task &getTask()
    {
        return m_Task;
    }

private:
    Task m_Task;
};

#endif // ENGINE_HPP
