#ifndef ENGINE_MASTER_HPP
#define ENGINE_MASTER_HPP

#include <algorithm>
#include <allocator/rootsim_allocator.hpp>
#include <scheduler/scheduler.hpp>
#include <service/service.hpp>
#include <vector>

class Master : public Service
{
public:
    explicit Master(const sid_t id, Scheduler<sid_t> *scheduler)
        : Service(id), m_Scheduler(scheduler), m_Links(new std::vector<sid_t>())
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
    void onTaskArrival(timestamp_t time, const Event *event) override;

    /**
     * @brief It adds a link to this master.
     *
     * @param linkId the link identifier
     */
    __attribute__((deprecated("Use addSlave"))) void addLink(const sid_t linkId)
    {
        // It checks if the link with the specified id has already been added to
        // the vector.
        if (std::find(m_Links->cbegin(), m_Links->cend(), linkId) !=
            m_Links->cend())
            die("Link %lu has already been added to the master %lu.",
                linkId,
                Service::getId());

        // Add the link identifier.
        m_Links->push_back(linkId);

        // Add a resource branch.
        m_Scheduler->addResource(linkId);
    }

    ENGINE_INLINE
    void addSlave(const sid_t slaveId)
    {
        // Register the slave identifier to the scheduler. With that,
        // the scheduler will be aware that the specified identifier
        // represents a slave and should be taken into account when
        // scheduling the tasks.
        m_Scheduler->addResource(slaveId);
    }

private:
    Scheduler<sid_t> *m_Scheduler;

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
    std::vector<sid_t> *m_Links;
};

#endif // ENGINE_MASTER_HPP
