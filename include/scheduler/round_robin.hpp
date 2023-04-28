#ifndef ENGINE_SCHEDULER_ROUND_ROBIN_HPP
#define ENGINE_SCHEDULER_ROUND_ROBIN_HPP

#include <core/core.hpp>
#include <queue>
#include <scheduler/scheduler.hpp>
#include <vector>

template <typename ResourceType>
class RoundRobin : public Scheduler<ResourceType>
{
    typedef Scheduler<ResourceType>                      scheduler;
    typedef typename scheduler::const_resource_reference const_resource_reference;

public:
    explicit RoundRobin() : m_NextResource(0U)
    {}

    /**
     * @brief It adds the resource to a circular queue.
     *
     * @param resource the resource to be added
     */
    ENGINE_INLINE void addResource(const_resource_reference resource) override
    {
        m_Resources.push_back(resource);
    }

    /**
     * @brief It schedules a resource in a round-robin manner.
     *
     * @return the scheduled resource
     */
    ENGINE_INLINE const_resource_reference schedule() override
    {
        /* Fetch the resource from the circular queue */
        const_resource_reference resource = m_Resources[m_NextResource];

        /* Maintain the circular queue property */
        m_NextResource = (m_NextResource + 1) % m_Resources.size();

        return resource;
    }

private:
    /**
     * @brief It represents a vector containing the resources to be scheduled
     *        in a round-robin manner.
     */
    std::vector<ResourceType> m_Resources;

    /**
     * @brief It contains the index in the resources vector representing the
     *        next resource to be scheduled.
     *
     *        This index is carefully incremented to not overflow the resources
     *        vector and to maintain the circular queue property.
     */
    typename std::vector<ResourceType>::size_type m_NextResource;
};

#endif // ENGINE_SCHEDULER_ROUND_ROBIN_HPP
