#ifndef ENGINE_MASTER_HPP
#define ENGINE_MASTER_HPP

#include <algorithm>
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
     * @brief It adds a link to this master.
     *
     * @param linkId the link identifier
     */
    void addLink(const sid_t linkId)
    {
        // It checks if the link with the specified id has already been added to the vector.
        if (std::find(m_Links.cbegin(), m_Links.cend(), linkId) != m_Links.cend())
            die("Link %lu has already been added to the master %lu.", linkId, Service::getId());

        // Add the link identifier.
        m_Links.push_back(linkId);

        // Add a resource branch.
        m_Scheduler->addResource(linkId);
    }

private:
    Scheduler<sid_t> *m_Scheduler;

    /**
     * @brief It represents a vector containing the link's identifiers
     *        which are connected with this master.
     *
     *        Viewing it as a graph, the link represents the edge connecting
     *        two nodes, being one of them this master.
     */
    std::vector<sid_t> m_Links;
};

#endif // ENGINE_MASTER_HPP
