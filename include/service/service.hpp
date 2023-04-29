#ifndef ENGINE_SERVICE_HPP
#define ENGINE_SERVICE_HPP

#include <engine.hpp>

class Service
{
public:
    /**
     * @brief Constructs a @p Service with the specified @p id.
     *
     * @details
     *        It is the caller's responsibility to ensure that
     *        the specified service's identifier is unique.
     *
     *        Otherwise, unexpected behavior may occur.
     *
     * @param id the service's id
     */
    explicit Service(const sid_t id)
        : m_Id(id) {}

    /**
     * @brief It processes the arrival of a task.
     *
     * @param time the time in which the task has arrived
     * @param t the task which arrived
     */
    virtual void onTaskArrival(timestamp_t time, const Task *t) = 0;

    /**
     * Returns the service's id.
     *
     * @return the service's id
     */
    sid_t getId() const { return m_Id; }
private:
    sid_t m_Id;
};

#endif // ENGINE_SERVICE_HPP
