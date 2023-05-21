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
    {
        m_Src         = static_cast<uint64_t>(-1);
        m_Dest        = static_cast<uint64_t>(-1);
        m_RouteOffset = 0ULL;
    }

    explicit Event(Task              task,
                   const uint64_t    src,
                   const uint64_t    dest,
                   const std::size_t routeOffset)
        : m_Task(task), m_Src(src), m_Dest(dest), m_RouteOffset(routeOffset)
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

    ENGINE_INLINE uint64_t getSource() const
    {
        return m_Src;
    }

    ENGINE_INLINE uint64_t getDestination() const
    {
        return m_Dest;
    }

    ENGINE_INLINE std::size_t getOffset() const
    {
        return m_RouteOffset;
    }

private:
    Task        m_Task;
    uint64_t    m_Src;
    uint64_t    m_Dest;
    std::size_t m_RouteOffset;
};

#endif // ENGINE_EVENT_HPP
