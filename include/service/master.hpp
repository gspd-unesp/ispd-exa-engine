#ifndef ENGINE_MASTER_HPP
#define ENGINE_MASTER_HPP

#include "core/core.hpp"
#include <algorithm>
#include <allocator/rootsim_allocator.hpp>
#include <scheduler/scheduler.hpp>
#include <service/service.hpp>
#include <vector>
#include <workload/workload.hpp>

struct MasterMetrics
{
    timestamp_t m_LastActivityTime;
    unsigned    m_CompletedTasks;
};

class Master : public Service
{
public:
    explicit Master(const sid_t id, Scheduler *scheduler)
        : Service(id), m_Scheduler(scheduler), m_Links(new std::vector<sid_t>())
    {
        scheduler->setMaster(this);
    }

    void onSchedulerInit(timestamp_t now);

    /**
     * @brief It schedules the specified task to a scheduled slave.
     *
     *        The slave is scheduled using some scheduling policy
     *        that has been previously configured to this master.
     *
     * @param time the time in which this task has arrived
     * @param t the task to be sent to the scheduled slave
     */
    void onTaskArrival(timestamp_t time, const Event *event) override;

    ENGINE_INLINE
    void addSlave(const sid_t slaveId)
    {
        // Register the slave identifier to the scheduler. With that,
        // the scheduler will be aware that the specified identifier
        // represents a slave and should be taken into account when
        // scheduling the tasks.
        m_Scheduler->addResource(slaveId);
    }

    ENGINE_INLINE
    const MasterMetrics &getMetrics() const
    {
        return m_Metrics;
    }

    ENGINE_INLINE
    Workload *getWorkload()
    {
        return m_Workload;
    }

public:
    Workload *m_Workload;

private:
    Scheduler *m_Scheduler;

    /**
     * @brief It represents a vector containing the link's identifiers
     *        which are connected with this master.
     *
     *        Viewing it as a graph, the link represents the edge connecting
     *        two nodes, being one of them this master.
     *
     * @details [ROOT-Sim]
     *        Currently, the model being simulation is represented by a static
     *        graph, i.e., a graph in which the vertex and edge set does not
     *        modify with time, therefore, constant. Thus, there is no need to
     *        use the ROOT-Sim memory allocators, since the vector content will
     *        not modify with the event processing.
     *
     *        However, if in any moment the vector need to be dynamic modified
     *        with the event processing. Then, the ROOT-Sim's memory allocator
     *        will need to be used.
     */

    // just for the master know who is his slaves.
    std::vector<sid_t> *m_Links;



    MasterMetrics m_Metrics{};
};

#endif // ENGINE_MASTER_HPP
