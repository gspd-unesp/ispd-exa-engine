#ifndef ENGINE_EVENT_HPP
#define ENGINE_EVENT_HPP

#include <core/core.hpp>
#include <customer/customer.hpp>
#include <routing/route.hpp>

/**
 * @brief An event (or message) is the smallest unit of information
 *        that is exchanged between service centers.
 */
struct Event
{
public:
    /**
     * @brief Construct an event holding the specified task.
     *
     * @param task the task
     */
    explicit Event(const Task &task) : m_Task(task), m_RouteDescriptor()
    {}

    /**
     * @brief Constructor which specifies the task and the
     *        route descriptor.
     *
     * @param task the task
     * @param routeDescriptor the route descriptor
     */
    explicit Event(const Task &task, const RouteDescriptor &routeDescriptor)
        : m_Task(task), m_RouteDescriptor(routeDescriptor)
    {}

    /**
     * @brief Returns a const (read-only) reference to the task.
     *
     * @return a const (read-only) reference to the task
     */
    ENGINE_INLINE const Task &getTask() const
    {
        return m_Task;
    }

    /**
     * @brief Return a const (read-only) reference to the route descriptor.
     *
     * @return a const (read-only) reference to the route descriptor
     */
    ENGINE_INLINE const RouteDescriptor &getRouteDescriptor() const
    {
        return m_RouteDescriptor;
    }

private:
    Task            m_Task;
    RouteDescriptor m_RouteDescriptor;
};

#endif // ENGINE_EVENT_HPP
