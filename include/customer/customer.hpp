    .#ifndef ENGINE_CUSTOMER_HPP
#define ENGINE_CUSTOMER_HPP

#include <core/core.hpp>
#include <cstdint>
#include <engine.hpp>

/// \brief Represents a customer, which is a unit of something that can be
///        processed by a resource.
///
/// The Customer class serves as a base class for entities that can be processed
/// by a resource in a system or program. It provides a common interface and
/// basic functionality for customer entities.
class Customer
{
};

/// \brief Enumerates the possible completion states of a task.
///
/// The TaskCompletionState enum class represents the different states a task
/// can be in during its lifecycle.
///
/// - JUST_GENERATED: This state indicates that the task has just been generated
///                   and is ready for processing. It signifies that the task
///                   has been created but has not yet been processed by any
///                   resource.
///
/// - PROCESSED: This state indicates that the task has been processed
///              successfully. It signifies that the task has completed its
///              execution and any associated operations or computations have
///              been performed.
///
/// The TaskCompletionState enum class provides a convenient way to track and
/// manage the completion status of tasks within a system or application. By
/// using this enum, developers can easily determine the current state of a task
/// and take appropriate actions based on its completion status.
enum class TaskCompletionState
{
    JUST_GENERATED,
    PROCESSED
};

/// \brief Represents a task, the smallest unit of work for a processing
/// resource.
///
/// The \c Task class is used to represent a task in a system or program. A task
/// is the fundamental unit of work that needs to be processed by a processing
/// resource, such as a CPU or a distributed processing system. Each task is
/// uniquely identified by a task identifier and has associated processing and
/// communication sizes.
///
/// \note Task Construction:
///       - The Task class provides constructors to initialize a task with its
///       task
///         identifier, origin master identifier, processing size, communication
///         size, and completion state.
///       - The caller is responsible for ensuring that the task identifier is
///       unique.
///       - It is recommended to use automatic task generators, such as workload
///         generators, that can generate unique task identifiers transparently.
class Task : public Customer
{
public:
    /// \brief Constructor that specifies the task identifier, origin master
    ///        identifier, processing size in megaflops, and communication size
    ///        in megabits of the task.
    ///
    /// \param tid The task identifier.
    /// \param origin The master's identifier who first scheduled this task.
    /// \param processingSize The processing size in megaflops.
    /// \param communicationSize The communication size in megabits.
    explicit Task(const uint64_t tid,
                  const sid_t    origin,
                  const double   processingSize,
                  const double   communicationSize) noexcept
        : m_Tid(tid), m_Origin(origin), m_ProcSize(processingSize),
          m_CommSize(communicationSize),
          m_CompletionState(TaskCompletionState::JUST_GENERATED)
    {}

    /// \brief Constructor that specifies the task identifier, origin master
    ///        identifier, processing size in megaflops, communication size in
    ///        megabits, and completion state.
    ///
    /// \param tid The task identifier.
    /// \param origin The master's identifier who first scheduled this task.
    /// \param processingSize The processing size in megaflops.
    /// \param communicationSize The communication size in megabits.
    /// \param completionState The completion state of the task.
    explicit Task(const uint64_t            tid,
                  const sid_t               origin,
                  const double              processingSize,
                  const double              communicationSize,
                  const TaskCompletionState completionState) noexcept
        : m_Tid(tid), m_Origin(origin), m_ProcSize(processingSize),
          m_CommSize(communicationSize), m_CompletionState(completionState)
    {}

    /// \brief Returns the processing size of the task in megaflops.
    ///
    /// \return The processing size in megaflops.
    ENGINE_INLINE double getProcessingSize() const
    {
        return m_ProcSize;
    }

    /// \brief Returns the communication size of the task in megabits.
    ///
    /// \return The communication size in megabits.
    ENGINE_INLINE double getCommunicationSize() const
    {
        return m_CommSize;
    }

    /// \brief Returns the unique identifier of the task.
    ///
    /// \return The task unique identifier.
    ENGINE_INLINE uint64_t getTid() const
    {
        return m_Tid;
    }

    /// \brief Returns the origin master identifier who first scheduled this
    ///        task.
    ///
    /// \return The origin master identifier.
    ENGINE_INLINE sid_t getOrigin() const
    {
        return m_Origin;
    }

    /// \brief Returns the completion state of the task.
    ///
    /// \return The task completion state.
    ENGINE_INLINE TaskCompletionState getCompletionState() const
    {
        return m_CompletionState;
    }

private:
    /// \brief Task identifier.
    ///
    /// The task identifier is a unique value used to precisely identify
    /// individual tasks within a distributed simulation environment. Each task
    /// is assigned a task identifier, allowing it to be tracked and referenced
    /// throughout the simulation.
    ///
    /// Task identifiers are generated using Szudzik's pairing function, which
    /// ensures their uniqueness in an efficient O(1) manner. This function
    /// combines two input values to produce a single output value that serves
    /// as the task identifier.
    ///
    /// To accommodate the scale of distributed simulations, the task identifier
    /// data type must be capable of holding a significant number of
    /// identifiers. A data type of at least 64 bits is typically used to ensure
    /// sufficient capacity.  Using a 64-bit task identifier data type ensures
    /// compatibility with the pairing function's output, which may require 64
    /// bits to represent all possible combinations.
    ///
    /// By utilizing a 64-bit task identifier data type, the simulation can
    /// handle an extensive range of tasks without sacrificing uniqueness or
    /// risking identifier collisions. The choice of a larger data type provides
    /// scalability and future-proofing, accommodating the potential growth and
    /// complexity of the simulation.
    ///
    uint64_t m_Tid;

    /// \brief Represents the identifier of the master that originated the task.
    ///
    /// The `m_Origin` member variable stores the identifier of the master
    /// that initially scheduled or created the task. It serves as a reference
    /// to identify the original source of the task within the simulation.
    ///
    /// In a distributed simulation environment, a task can be scheduled and
    /// processed by multiple masters as it traverses the simulation network.
    /// However, the `m_Origin` identifier points to the master that first
    /// scheduled the task.
    ///
    /// This information is valuable because it allows intermediate masters
    /// along the route to determine whether the task should be forwarded to the
    /// next master or processed locally as if it were their own task. By
    /// inspecting the `m_Origin` identifier, masters can make informed
    /// decisions about task handling based on its origin.
    ///
    /// It is important to note that the `m_Origin` identifier remains
    /// constant throughout the task's journey from the source to its final
    /// destination. Even if multiple masters participate in scheduling and
    /// forwarding the task, this identifier refers to the original master that
    /// initiated the task.
    ///
    /// \note While the `m_Origin` identifier provides valuable context about
    /// the task's origin, it does not necessarily indicate the current master
    /// processing or handling the task. Use other relevant member variables or
    /// functions to obtain information about the current master.
    ///
    sid_t m_Origin;

    /// \brief Task Processing Size (in megaflops).
    ///
    /// The task processing size represents the computational resource required
    /// to complete this task. It is measured in megaflops, which stands for
    /// "millions of floating-point operations per second". The processing size
    /// indicates the amount of computational work that needs to be performed by
    /// the task.
    ///
    /// The value of the processing size is a numerical quantity that specifies
    /// the amount of megaflops required to execute the task. This metric helps
    /// estimate the execution time and resource utilization of the task in a
    /// distributed simulation environment. By knowing the processing size, the
    /// scheduler or resource manager can make informed decisions about task
    /// allocation and scheduling, taking into account the available
    /// computational resources.
    ///
    /// It's important to note that the processing size is an abstract measure
    /// and may vary depending on the nature of the task and the simulation
    /// context. Tasks with higher processing sizes require more computational
    /// resources and may take longer to complete than tasks with lower
    /// processing sizes.
    ///
    /// The processing size is typically determined based on the task's
    /// computational requirements and characteristics, such as the complexity
    /// of the algorithm, the amount of data to be processed, or the intensity
    /// of numerical calculations involved.
    ///
    /// By considering the processing size of tasks, the simulation system can
    /// optimize resource allocation, load balancing, and scheduling strategies
    /// to achieve efficient utilization of computational resources and improve
    /// overall simulation performance.
    ///
    double m_ProcSize;

    /// \brief Task Communication Size (in megabits).
    ///
    /// The communication size indicates the required network bandwidth
    /// necessary to transmit the task from its source to its destination in the
    /// modeled network. It represents the volume of data that needs to be
    /// transferred, exchanged, or shared during the execution of the task.
    ///
    /// The value of the communication size is a numerical quantity that
    /// specifies the amount of megabits needed for the task's communication.
    /// This metric helps estimate the communication overhead and network
    /// resource utilization associated with the task.
    ///
    /// It's important to consider the communication size when designing and
    /// optimizing distributed simulations, as it directly impacts the network
    /// performance and the overall efficiency of the simulation. Tasks with
    /// larger communication sizes may require higher network bandwidth and may
    /// experience increased latency or congestion.
    ///
    /// By accounting for the communication size of tasks, simulation systems
    /// can optimize network resources, communication patterns, routing
    /// strategies, and synchronization mechanisms to minimize network
    /// congestion, reduce communication latency, and improve overall simulation
    /// performance.
    ///
    double m_CommSize;

    /// \brief Represents the completion state of a task.
    ///
    /// The TaskCompletionState enumeration is used to indicate the state of
    /// task completion in a program or system. It provides a way to track the
    /// progress or status of a task during its lifecycle.
    ///
    /// The possible completion states are:
    ///   - JUST_GENERATED: Represents the state of a task that has just been
    ///                     generated or created.
    ///
    ///                     This state indicates that the task has been newly
    ///                     generated and has not yet undergone any processing
    ///                     or execution.
    ///
    ///   - PROCESSED: Represents the state of a task that has been processed or
    ///                executed.
    ///
    ///                This state indicates that the task has undergone some
    ///                form of processing, computation, or execution.
    TaskCompletionState m_CompletionState;
};

#endif // ENGINE_CUSTOMER_HPP
