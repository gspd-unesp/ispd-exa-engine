#ifndef ENGINE_SCHEDULER_ROUND_ROBIN_HPP
#define ENGINE_SCHEDULER_ROUND_ROBIN_HPP

#include <core/core.hpp>
#include <cstdint>
#include <queue>
#include <scheduler/scheduler.hpp>
#include <vector>

/// \class RoundRobin
///
/// \brief A scheduling algorithm that assigns resources in a round-robin
///        manner.
///
/// The `RoundRobin` class implements a scheduling algorithm known as
/// round-robin, where resources are assigned or scheduled in a circular order.
/// Each resource is given a turn in a cyclic fashion, ensuring fair
/// distribution of resources and preventing any single resource from
/// monopolizing the scheduling process.
///
/// The `RoundRobin` scheduler maintains a circular queue of resources and
/// schedules them based on the order of the queue. When a resource is
/// scheduled, it is removed from the front of the queue and placed at the end,
/// ensuring that each resource gets an equal opportunity to be scheduled.
class RoundRobin : public Scheduler
{
public:
    /// \brief Default constructor.
    RoundRobin() : m_NextResource(0U)
    {}

    /// \brief Function called during the initialization phase of the scheduler.
    ///
    /// In the initialization phase of the scheduler, the `RoundRobin` algorithm
    /// schedules N tasks, where N is the number of resources in the system.
    void onInit() override;

    /// \brief Function called when a task is completed by a resource in the
    ///        Round-Robin scheduler.
    ///
    /// When a task is completed by a resource in the Round-Robin scheduler, the
    /// `onCompletedTask` function is called to handle the completion event.
    /// This function specifically implements the behavior for handling task
    /// completion in the Round-Robin scheduling algorithm.
    ///
    /// \param now The current time at which the task is completed.
    /// \param slaveId The identifier of the slave resource that completed the
    ///                task.
    /// \param task The completed task object containing relevant
    ///             information.
    void onCompletedTask(const double now,
                         const sid_t  slaveId,
                         const Task  &task) override;

    /// \brief Adds a resource to the circular queue.
    ///
    /// \param resource The resource to be added to the circular queue.
    ///
    /// This function adds a resource to the circular queue maintained by the
    /// `RoundRobin` scheduler. The resource is appended to the end of the
    /// queue, and it will be scheduled according to the round-robin algorithm.
    void addResource(const sid_t resource) override
    {
        m_Resources.push_back(resource);
    }

    /// \brief Schedules the next resource in a round-robin manner.
    ///
    /// This function implements the round-robin scheduling algorithm to
    /// determine the next resource to be scheduled. It fetches the resource at
    /// the front of the circular queue, updates the queue by moving the next
    /// resource to the end, and returns the fetched resource.
    ///
    /// \note The way this circular queue is managed is done `virtually` none of
    ///       removing at the head or append to the tail of the queue is made.
    ///       Basically, an index is traversing the queue indicating which
    ///       resource is the next to be scheduled.
    ///
    /// \note The circular queue is maintained using the `m_NextResource` member
    ///       variable, which represents the index of the next resource to be
    ///       scheduled. It is incremented by one modulo the number of resources
    ///       in the queue to ensure it stays within the valid range and
    ///       maintains the circular queue property.
    ///
    /// \return The scheduled resource.
    uint64_t schedule() override
    {
        // Fetch the resource from the circular queue.
        uint64_t resource = m_Resources[m_NextResource];

        // Maintain the circular queue property. Thta is, if the next
        // addition to the `m_NextResource` past the last index of
        // the queue, then the `m_NextResource` turns to 0.
        m_NextResource = (m_NextResource + 1) % m_Resources.size();

        return resource;
    }

private:
    /// \brief A vector containing the resources to be scheduled in a
    ///        round-robin manner.
    std::vector<sid_t> m_Resources;

    /// \brief The index representing the next resource to be scheduled in the
    ///        circular queue.
    ///
    /// \note This index is carefully incremented to prevent overflow and ensure
    ///       that it wraps around to the beginning of the circular queue when
    ///       it reaches the end. It maintains the circular queue property of
    ///       the round-robin scheduling algorithm.
    typename std::vector<sid_t>::size_type m_NextResource;
};

#endif // ENGINE_SCHEDULER_ROUND_ROBIN_HPP
