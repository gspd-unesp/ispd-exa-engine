#ifndef ENGINE_SCHEDULER_HPP
#define ENGINE_SCHEDULER_HPP

template <typename ResourceType>
class Scheduler
{
public:
    typedef const ResourceType& const_resource_reference;

    /**
     * @brief It adds a resource to be scheduled.
     *
     * @param resource the resource
     */
    virtual void addResource(const_resource_reference resource) = 0;

    /**
     * @brief It schedules the next resource by some scheduling policy.
     *
     * @return the scheduled resource
     */
    virtual const_resource_reference schedule() = 0;
};

#endif // ENGINE_SCHEDULER_HPP
