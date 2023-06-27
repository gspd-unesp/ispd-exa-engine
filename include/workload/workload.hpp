#ifndef ENGINE_WORKLOAD_HPP
#define ENGINE_WORKLOAD_HPP

#include <ROOT-Sim.h>
#include <core/core.hpp>
#include <customer/customer.hpp>
#include <workload/distribution.hpp>

/// \class Workload
///
/// \brief A base class representing a workload.
///
/// The `Workload` class serves as a base class for representing a workload,
/// which represents a set of tasks that need to be processed or executed. This
/// class provides the common functionality and properties of a workload,
/// including the total number of tasks and methods to access and manipulate the
/// workload.
class Workload
{
public:
    /// \brief Constructs a `Workload` object with the specified number of
    ///        tasks.
    ///
    /// \param taskAmount The total number of tasks in the workload.
    /// \param interarrivalDistribution The interarrival task time distribution.
    explicit Workload(const int             taskAmount,
                      WorkloadDistribution *interarrivalDistribution)
        : m_TaskAmount(taskAmount),
          m_InterarrivalDistribution(interarrivalDistribution)
    {}

    /// \brief Sets the workload for a task by assigning processing and
    ///        communication sizes.
    ///
    /// \details The `setTaskWorkload` function is a pure virtual function,
    ///          which means it must be overridden by derived classes. It sets
    ///          the workload for a task by assigning processing and
    ///          communication sizes based on the specific implementation of the
    ///          derived class.
    ///
    /// \param processingSize A reference to a double variable to store the
    ///                       processing size.
    /// \param communicationSize A reference to a double variable to store the
    ///                          communication size.
    virtual void setTaskWorkload(double &processingSize,
                                 double &communicationSize) = 0;

    /// \brief Set the arrival time of a task based on the specified
    ///        interarrival task distribution.
    ///
    /// This function sets the arrival time of a task using the provided
    /// `arrivalTime` value and the interarrival task distribution associated.
    ///
    /// The `arrivalTime` parameter represents the time at which the task is
    /// scheduled to arrive. The exact method of setting the arrival time is
    /// delegated to the interarrival task distribution stored in the
    /// `m_InterarrivalDistribution` member variable.
    ///
    /// Example usage:
    /// ```
    /// double arrivalTime = 10.0; // Specify the arrival time
    /// setTaskArrivalTime(arrivalTime); // Set the task arrival time using the
    ///                                  // interarrival distribution
    /// ```
    ///
    /// \note For simplicity, let's consider the interarrival task time
    ///       distribution as a `FixedWorkloadDistribution` with an offset of
    ///       `5.0`, meaning that the interarrival time is constant. In this
    ///       scenario, if the initial value of `arrivalTime` is `10`, after
    ///       applying this method, the `arrivalTime` will be updated to `15`.
    ///
    ///
    /// This example demonstrates how to use the `setTaskArrivalTime` function
    /// to assign an arrival time to a task based on the defined interarrival
    /// distribution.
    ///
    /// \param arrivalTime The arrival time of the task to be set.
    void setTaskArrivalTime(double &arrivalTime)
    {
        m_InterarrivalDistribution->setArrivalTime(arrivalTime);
    }

    /// \brief Retrieves the number of remaining tasks in the workload.
    ///
    /// The `getRemainingTasks` function returns the number of tasks that are
    /// yet to be processed or executed in the workload.
    ///
    /// \return The number of remaining tasks in the workload.
    ENGINE_INLINE int getRemainingTasks()
    {
        return m_TaskAmount;
    }

    /// \brief Checks if there is remaining workload.
    ///
    /// The `hasRemainingWorkload` function checks if there are any remaining
    /// tasks in the workload that need to be processed or executed.
    ///
    /// \details Generally, this function is used by the master to check whether
    ///          there are remaining tasks to be executed.
    ///
    /// \return `true` if there are remaining tasks in the workload, `false`
    ///         otherwise.
    ENGINE_INLINE bool hasRemainingWorkload()
    {
        return m_TaskAmount > 0;
    }

protected:
    /// \brief The total number of tasks in the workload.
    ///
    /// \details The `m_TaskAmount` variable represents the total number of
    ///          tasks in the workload. It is a signed integer data type that
    ///          stores the count of tasks. Although an unsigned data type could
    ///          have been used, a signed integer was preferred in this case.
    ///          The reason behind this decision is that the `setTaskWorkload`
    ///          function does not perform any checking on whether there are
    ///          remaining tasks. Therefore, it is the responsibility of the
    ///          caller to ensure that the workload is properly managed and
    ///          tasks are generated only when there are remaining tasks. In the
    ///          event that the caller mistakenly generates a task without
    ///          checking for remaining tasks, using a signed integer allows the
    ///          `m_TaskAmount` to become negative instead of wrapping around to
    ///          the maximum value of an unsigned data type. This provides a
    ///          clear indication of an error or an invalid state, making it
    ///          easier to detect and handle such scenarios.
    int m_TaskAmount;

    /// \brief Pointer to the interarrival task time distribution.
    ///
    /// This pointer represents the interarrival task time distribution.
    /// The distribution determines the pattern or model of time intervals
    /// between the arrivals of tasks in a workload.
    ///
    /// It is important to note that this pointer does not hold an actual
    /// instance of the `WorkloadDistribution` class by default. Before using
    /// the `m_InterarrivalDistribution` pointer, it needs to be properly
    /// assigned to a valid object of type `WorkloadDistribution`. Ensure that
    /// the pointer is initialized correctly to avoid potential errors or
    /// undefined behavior, since `WorkloadDistribution` is an abstract class.
    WorkloadDistribution *m_InterarrivalDistribution;
};

/// \class ConstantWorkload
///
/// \brief A class representing a constant workload for a simulation.
///
/// The ConstantWorkload class is derived from the Workload base class and
/// represents a workload with a fixed number of tasks, constant processing
/// size, and constant communication size. Each task in the workload has the
/// same processing and communication size.
///
/// Example usage:
/// ```
/// ConstantWorkload workload(10, 100.0, 50.0);
/// double processingSize, communicationSize;
/// workload.setTaskWorkload(processingSize, communicationSize);
/// ```
///
/// In the above example, a ConstantWorkload object is created with a task
/// amount of 10, processing size of 100.0, and communication size of 50.0. The
/// `setTaskWorkload` method is then called to retrieve the processing and
/// communication sizes for each task in the workload.
class ConstantWorkload : public Workload
{
public:
    /// \brief Constructs a ConstantWorkload object with the specified task
    /// amount, processing size, and communication size.
    ///
    /// \param taskAmount The number of tasks in the workload.
    /// \param processingSize The constant processing size for each task (in
    ///                       megaflops).
    /// \param communicationSize The constant communication size for
    ///                          each task (in megabits).
    /// \param interarrivalDistribution The interarrival task time distribution.
    explicit ConstantWorkload(const int             taskAmount,
                              const double          processingSize,
                              const double          communicationSize,
                              WorkloadDistribution *interarrivalDistribution)
        : Workload(taskAmount, interarrivalDistribution),
          m_ProcessingSize(processingSize),
          m_CommunicationSize(communicationSize)
    {}

    /// \brief Sets the workload for a single task.
    ///
    /// Sets the processing size and communication size for a single task in the
    /// workload. The processing size and communication size are set to the
    /// constant values specified during object construction.
    ///
    /// \param processingSize A reference to a double variable to store the
    ///                       processing size.
    /// \param communicationSize A reference to a double variable to store the
    ///                          communication size.
    ENGINE_INLINE void setTaskWorkload(double &processingSize,
                                       double &communicationSize) override
    {
        processingSize    = m_ProcessingSize;
        communicationSize = m_CommunicationSize;
        m_TaskAmount--;
    }

private:
    /// \brief The constant processing size (in megaflops) for each task.
    double m_ProcessingSize;
    /// \brief The constant communication size (in megabits) for each task.
    double m_CommunicationSize;
};

/// \class UniformRandomWorkload
///
/// \brief A class representing a workload with uniformly random processing and
///        communication sizes for each task.
///
/// The `UniformRandomWorkload` class allows generating workloads with a
/// specified number of tasks, where each task has random processing and
/// communication sizes. The processing and communication sizes are uniformly
/// distributed within the specified range for each task.
///
/// Example usage:
/// ```
/// UniformRandomWorkload workload(10, 100.0, 200.0, 450.0, 500.0);
/// double processingSize, communicationSize;
/// workload.setTaskWorkload(processingSize, communicationSize);
/// ```
///
/// In the above example, a UniformRandomWorkload object is created with a task
/// amount of 10, with a processing size uniform randomly distributed through
/// the interval [100, 200] and communication size uniform randomly distributed
/// through the interval [450.0, 500.0]. The `setTaskWorkload` method is then
/// called to retrieve the processing and communication sizes for each task in
/// the workload.
class UniformRandomWorkload : public Workload
{
public:
    /// \brief Constructs a `UniformRandomWorkload` object with the specified
    ///        parameters.
    ///
    /// \param taskAmount The total number of tasks in the workload.
    /// \param minProcessingSize The minimum value for the processing size (in
    ///                          megaflops) of each task.
    /// \param maxProcessingSize The maximum value for the processing size (in
    ///                          megaflops) of each task.
    /// \param minCommunicationSize The minimum value for the communication size
    ///                             (in megabits) of each task.
    /// \param maxCommunicationSize The maximum value for the communication size
    ///                             (in megabits) of each task.
    /// \param interarrivalDistribution The interarrival task time distribution.
    explicit UniformRandomWorkload(
        const int             taskAmount,
        const double          minProcessingSize,
        const double          maxProcessingSize,
        const double          minCommunicationSize,
        const double          maxCommunicationSize,
        WorkloadDistribution *interarrivalDistribution)
        : Workload(taskAmount, interarrivalDistribution),
          m_MinProcessingSize(minProcessingSize),
          m_MaxProcessingSize(maxProcessingSize),
          m_MinCommunicationSize(minCommunicationSize),
          m_MaxCommunicationSize(maxCommunicationSize)
    {}

    /// \brief Sets the workload for a task by generating random processing and
    ///        communication sizes.
    ///
    /// The `setTaskWorkload` function assigns random processing and
    /// communication sizes to a task. The sizes are uniformly distributed
    /// within the range specified during the object construction
    /// (`minProcessingSize` to `maxProcessingSize` for processing size, and
    /// `minCommunicationSize` to `maxCommunicationSize` for communication
    /// size).
    ///
    /// \param processingSize A reference to a double variable to store the
    ///                       processing size.
    /// \param communicationSize A reference to a double variable to store the
    ///                          communication size.
    void setTaskWorkload(double &processingSize,
                         double &communicationSize) override
    {
        processingSize =
            Random() * (m_MaxProcessingSize - m_MinProcessingSize) +
            m_MinProcessingSize;
        communicationSize =
            Random() * (m_MaxCommunicationSize - m_MinCommunicationSize) +
            m_MinCommunicationSize;
        m_TaskAmount--;
    }

private:
    /// \brief The minimum value for the processing size (in megaflops) of each
    ///        task.
    double m_MinProcessingSize;
    /// \brief The maximum value for the processing size (in megaflops) of each
    ///        task.
    double m_MaxProcessingSize;
    /// \brief The minimum value for the communication size (in megabits) of
    ///        each task.
    double m_MinCommunicationSize;
    /// \brief The maximum value for the communication size (in megabits) of
    ///        each task.
    double m_MaxCommunicationSize;
};

#endif // ENGINE_WORKLOAD_HPP
