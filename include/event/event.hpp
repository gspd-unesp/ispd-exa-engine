#ifndef ENGINE_EVENT_HPP
#define ENGINE_EVENT_HPP

#include <core/core.hpp>
#include <customer/customer.hpp>

/**
 * @brief An event (or message) is the smallest unit of information
 *        that is exchanged between service centers.
 */
struct Event
{
public:
    /**
     * @brief Constructor an event holding the specified task.
     *
     * @param task the task carried by the event
     */
    explicit Event(Task task) : m_Task(task)
    {}

    /**
     * @brief Returns a const (read-only) reference to the task.
     *
     * @return a const (read-only) reference to the task
     */
    ENGINE_INLINE const Task &getTask()
    {
        return m_Task;
    }

private:
    Task m_Task;
};

#endif // ENGINE_EVENT_HPP
