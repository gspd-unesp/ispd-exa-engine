#ifndef ENGINE_SCHEDULER_HPP
#define ENGINE_SCHEDULER_HPP

#include <core/core.hpp>
#include <cstdint>
#include <customer/customer.hpp>

class Master;

/// \class Scheduler
///
/// \brief Abstract base class for schedulers.
///
/// The `Scheduler` class provides an abstract interface for schedulers,
/// defining the common operations and behaviors expected from any scheduler
/// implementation. It serves as a base class that other scheduler classes can
/// inherit from to implement specific scheduling policies and algorithms.
///
/// Schedulers are responsible for managing and coordinating the allocation of
/// tasks to processing resources for a system or application.
///
/// \note The `Scheduler` class is an abstract base class and cannot be
///       instantiated directly. Instead, specific scheduler implementations
///       should derive from this class and provide concrete implementations for
///       the pure virtual functions.
class Scheduler
{
public:
    /// \brief Initializes the scheduler.
    ///
    /// The `onInit` function is called to perform any necessary initialization
    /// steps when the scheduler is just to be started. Subclasses should
    /// implement this function to provide their specific initialization logic.
    virtual void onInit() = 0;

    /// \brief Function called when a task is completed by a resource.
    ///
    /// When a task is completed by a resource, the `onCompletedTask` function
    /// is called by the scheduler to handle the completion event. This function
    /// allows the scheduler to perform any necessary actions or updates based
    /// on the completed task, such as tracking statistics, adjusting scheduling
    /// decisions, or handling resource-specific logic.
    ///
    /// \param now The current time at which the task is completed.
    /// \param resourceId The identifier of the resource that completed the
    ///                   task.
    /// \param task The completed task object containing relevant
    ///             information.
    ///
    /// \note This function is pure virtual, meaning that it is an abstract
    ///       function in the `Scheduler` base class and must be implemented in
    ///       derived classes. Each specific scheduling algorithm can provide
    ///       its own implementation of this function to define the behavior and
    ///       actions to be taken when a task is completed. This allows for
    ///       flexibility and customization based on the requirements and
    ///       characteristics of the scheduler.
    virtual void onCompletedTask(const double   now,
                                 const uint64_t resourceId,
                                 const Task    &task) = 0;

    /// \brief Adds a resource to be scheduled.
    ///
    /// \param resource The resource to be added to the scheduler.
    ///
    /// This function adds a resource to the scheduler, allowing it to be
    /// included in the scheduling process. The specific behavior of adding a
    /// resource may vary depending on the scheduling policy implemented by the
    /// subclass.
    virtual void addResource(uint64_t resource) = 0;

    /// \brief Schedules the next resource based on the scheduling policy.
    ///
    /// \return The scheduled resource.
    ///
    /// This function determines the next resource to be scheduled based on the
    /// scheduling policy implemented by the subclass. The specific scheduling
    /// policy may consider various factors such as resource availability,
    /// workload, or priority.
    virtual uint64_t schedule() = 0;

    /// \brief Sets the master for the scheduler.
    ///
    /// \param master A pointer to the master object.
    ///
    /// This function sets the master object for the scheduler, allowing it to
    /// communicate with the master and receive instructions. It is important to
    /// ensure that the provided master pointer is not a nullptr, as this would
    /// lead to undefined behavior. If a nullptr is passed, the program is
    /// immediately aborted.
    ///
    /// \note This function does not take ownership of the master object.
    ///       It is the caller's responsibility to ensure the lifetime of the
    ///       master object extends beyond the scheduler's usage.
    ENGINE_INLINE void setMaster(Master *master)
    {
        /// Check if the master being set to this scheduler is nullptr.
        /// If so, the program is immediately aborted.
        if (UNLIKELY(!master))
            die("Setting a nullptr master to a scheduler.");
        m_Master = master;
    }

protected:
    /// \brief Pointer to the master object.
    ///
    /// The `m_Master` member variable holds a pointer to the master object
    /// associated with the scheduler. It allows the scheduler to communicate
    /// with the master and receive instructions or perform actions as needed.
    /// The master object is responsible for coordinating and managing the
    /// scheduling process.
    ///
    /// \note This member variable is typically set using the `setMaster`
    ///       function to ensure proper initialization. It is important to
    ///       ensure that the `m_Master` pointer is valid and points to a valid
    ///       master object before accessing it.
    Master *m_Master;
};
#endif // ENGINE_SCHEDULER_HPP
