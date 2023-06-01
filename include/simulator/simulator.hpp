#ifndef ENGINE_SIMULATOR_HPP
#define ENGINE_SIMULATOR_HPP

#include <core/core.hpp>
#include <functional>
#include <memory>
#include <service/service.hpp>
#include <unordered_map>

namespace ispd::sim
{

/// \enum SimulationMode
///
/// \brief Enumeration for simulation modes.
///
/// The \c SimulationMode enumeration is used to select the desired mode for the
/// simulation. Each mode represents a different approach to progress the
/// simulation and affects how the simulator works internally.
///
/// Available simulation modes:
/// - SEQUENTIAL: The simulation progresses sequentially without any
///   concurrency.
///
/// - OPTIMISTIC: The optimistic mode uses the Time Warp mechanism for parallel
///   and optimistic execution of events. It allows for speculative execution
///   and rollback when necessary.
///
/// - CONSERVATIVE: The conservative mode ensures conservative execution of
///   events by using synchronization mechanisms to prevent causality
///   violations.
enum class SimulationMode
{
    SEQUENTIAL,
    OPTIMISTIC,
    CONSERVATIVE
};

/// \enum SimulatorType
///
/// \brief Enumeration for simulator types.
///
/// The \c SimulatorType enumeration is used to specify the underlying simulator
/// being used to progress the simulation. It provides a way to categorize and
/// identify different types of simulators that can be used in the simulation
/// environment.
///
/// Available simulator types:
/// - ROOTSIM: Represents the root simulator type. This can be the main
///            simulator or the top-level simulator used for running the
///            simulation.
enum class SimulatorType
{
    ROOTSIM
};

/// \class Simulator
///
/// \brief Base simulator class.
///
/// The Simulator class serves as the foundation for all underlying simulator
/// engines and simulation modes. It provides essential functionality for
/// registering functions to customize the initialization and finalization of
/// services. By using parameterized functions, users can define their own
/// behavior for service initialization and finalization.
class Simulator
{
public:
    /// \brief Register a service initializer for a service with the specified
    ///        identifier.
    ///
    /// The service initializer is a parameterized piece of code that is
    /// executed when the service with the specified identifier is initialized
    /// by the simulation. This function allows the user to modify the
    /// initialization behavior of the service by providing a custom
    /// implementation through the \c serviceInitializer function.
    ///
    /// \param serviceId The identifier of the service.
    /// \param serviceInitializer The function that represents the service
    ///                           initializer.
    ///
    /// \note If a service initializer with the same \c serviceId has already
    ///       been registered, the program will abort.
    ENGINE_INLINE void registerService(
        const sid_t                       serviceId,
        const std::function<Service *()> &serviceInitializer)
    {
        // It checks if a service initializer with that id has already been
        // registered. If so, then the program is immediately aborted.
        if (m_ServiceInitializers.find(serviceId) !=
            m_ServiceInitializers.end())
            die("A service with id %lu has already been registered.",
                serviceId);

        // Register the service.
        m_ServiceInitializers.insert(
            std::make_pair(serviceId, serviceInitializer));
    }

    /// \brief Register a service finalizer for a service with the specified
    ///        identifier.
    ///
    /// The service finalizer is a parameterized piece of code tthat is executed
    /// when the service with the specified identifier is finalized by the
    /// simulation. This function allows the user to modify the finalization
    /// behavior of the service by providing a custom implementation through the
    /// \c serviceFinalizer function.
    ///
    /// \param serviceId The identifier of the service.
    /// \param serviceFinalizer The function that represents the service
    ///                         finalizer.
    ///
    /// \note If a service finalizer with the same \c serviceId has already been
    ///       registered, the program will abort.
    ENGINE_INLINE void registerServiceFinalizer(
        const sid_t                           serviceId,
        const std::function<void(Service *)> &serviceFinalizer)
    {
        // It checks if a service finalizer with that id has already been
        // registered. If so, then the program is immediately aborted.
        if (m_ServiceFinalizers.find(serviceId) != m_ServiceFinalizers.end())
            die("A service with id %lu has already been registered.",
                serviceId);

        // Register the service finalizer.
        m_ServiceFinalizers.insert(std::make_pair(serviceId, serviceFinalizer));
    }

    /// \brief Execute the simulation.
    virtual void simulate() = 0;

    /// \brief Get a const (read-only) reference to the map of service
    ///        initializers.
    ///
    /// This function returns a const (read-only) reference to the map of
    /// service initializers, which contains the registered service initializers
    /// for different service identifiers.
    ///
    /// \return A const (read-only) reference to the map of service
    ///         initializers.
    ENGINE_INLINE const std::unordered_map<sid_t, std::function<Service *()>>                     &
    getServices()
    {
        return m_ServiceInitializers;
    }

    /// \brief Get a const (read-only) reference to the map of service
    ///        finalizers.
    ///
    /// This function returns a const (read-only) reference to the map of
    /// service finalizers, which contains the registered service finalizers for
    /// different service identifiers.
    ///
    /// \return A const (read-only) reference to the map of service finalizers.
    ENGINE_INLINE const std::unordered_map<sid_t,
                                           std::function<void(Service *)>>                     &
    getServicesFinalizers()
    {
        return m_ServiceFinalizers;
    }

protected:
    /// \brief It contains code sections that will be called when a service with
    ///        the respective identifier is initialized.
    ///
    /// These code sections represent parameterized functions that is registered
    /// by the user to customize the service's initialization behavior.
    std::unordered_map<sid_t, std::function<Service *()>>
        m_ServiceInitializers{};

    /// \brief It contains code sections that will be called when a service with
    ///        the respective identifier is finalized.
    ///
    /// These code sections represent parameterized functions that is registered
    /// by the user to customize the service's finalization behavior.
    std::unordered_map<sid_t, std::function<void(Service *)>>
        m_ServiceFinalizers{};
};

/// \class SimulatorBuilder
///
/// \brief A builder class for configuring and creating Simulator objects.
///
/// The SimulatorBuilder class provides a convenient way to configure and create
/// Simulator objects for running simulations. It allows users to specify the
/// simulation engine type, simulation mode, number of threads, checkpointing
/// interval, core binding, and Global Virtual Time (GVT) calculation period,
/// among others.
///
/// After configuring the desired parameters, users can call the
/// `createSimulator` member function to obtain a \c Simulator object that
/// represents the configured simulation environment. The \c Simulator object
/// can then be used to execute the simulation.
///
/// Example usage:
/// \code
/// SimulatorBuilder builder(SimulatorType::ROOTSIM,
/// SimulationMode::OPTIMISTIC);
///
/// builder.setThreads(4);
/// builder.setCheckpointInterval(256);
/// builder.setCoreBinding(true);
/// builder.setGvtPeriod(1000);
///
/// Simulator *simulator = builder.createSimulator();
/// simulator->simulate();
/// \endcode
///
/// \details It is important to configure the parameters appropriately based on
///          the requirements of the simulation to achieve the desired behavior
///          and performance.
class SimulatorBuilder
{
public:
    /// \brief Constructs a SimulatorBuilder with the specified simulation
    ///        engine and mode.
    ///
    /// This function constructs a SimulatorBuilder object with the specified
    /// simulation engine type and simulation mode. The simulation engine type
    /// is represented by the \c SimulatorType enumeration, which determines the
    /// underlying simulator being used. The simulation mode, represented by the
    /// \c SimulationMode enumeration, selects the desired behavior of the
    /// simulation, such as sequential, optimistic (Time Warp), or conservative.
    /// The constructed SimulatorBuilder object can then be used to configure
    /// and build the simulator for the specified engine and mode.
    ///
    /// \param type The underlying simulator type, such as ROOTSIM.
    /// \param mode The simulation mode, such as SEQUENTIAL, OPTIMISTIC, or
    ///             CONSERVATIVE.
    explicit SimulatorBuilder(const SimulatorType  type,
                              const SimulationMode mode)
        : m_Type(type), m_Mode(mode)
    {}

    /// \brief Set the amount of threads to be used in the simulation.
    ///
    /// This function sets the number of threads to be used in the simulation.
    /// The simulation will distribute the workload across the specified number
    /// of threads, potentially improving performance through parallel
    /// execution.
    ///
    /// However, increasing the number of therads does not necessarily improves
    /// the simulation performance, since the synchronization cost may outweight
    /// the commitment of events (progress of the simulation).
    ///
    /// Therefore, this value must be chosen carefully. Since, it may degrade
    /// the simulation performance.
    ///
    /// \param threads The amount of threads used in the simulation.
    ///
    /// \return A reference to the current \c SimulatorBuilder object, allowing
    ///         method chaining for further configuration.
    SimulatorBuilder &setThreads(const uint32_t threads);

    /// \brief Set the checkpointing interval of the logical processes states.
    ///
    /// This function sets the interval at which the state of a logical process
    /// will be saved during the simulation. After processing a specified number
    /// of events, the state of each logical process will be saved as a
    /// checkpoint.
    ///
    /// Checkpointing allows for recovery of the consistent logical process
    /// state and, therefore, the simulation state after the receiving of a
    /// straggler message or anti-message.
    ///
    /// \param interval The amount of events to be processed before the state
    ///                 of a logical process be saved.
    ///
    /// \return A reference to the current \c SimulatorBuilder object, allowing
    ///         method chaining for further configuration.
    SimulatorBuilder &setCheckpointInterval(const uint32_t interval);

    /// \brief Enable or disable thread-to-core binding.
    ///
    /// This function allows you to enable or disable the binding of a thread to
    /// a specific core. When thread-to-core binding is enabled, the operating
    /// system ensures that the thread executes consistently on the assigned
    /// core(s) or CPU(s) whenever it is scheduled. If it is disabled, the
    /// operating system has the freedom to schedule the thread on any available
    /// core.
    ///
    /// \param coreBinding If true, the thread-to-core binding is enabled. If
    /// false, it is disabled.
    ///
    /// \return A reference to the current \c SimulatorBuilder object, allowing
    ///         method chaining for further configuration.
    SimulatorBuilder &setCoreBinding(const bool coreBinding);

    /// \brief Set the calculation period for Global Virtual Time (GVT).
    ///
    /// This function sets the calculation period for determining the Global
    /// Virtual Time (GVT) in the optimistic parallel discrete event simulation.
    /// GVT represents the smallest timestamp known to all processors in the
    /// simulation, indicating that all causally dependent events with smaller
    /// timestamps have been processed.
    ///
    /// \param period The calculation period in microseconds. It determines the
    ///               interval at which GVT is recalculated.
    ///
    /// \return A reference to the current \c SimulatorBuilder object, allowing
    ///         method chaining for further configuration.
    SimulatorBuilder &setGvtPeriod(const uint32_t period);

    /// \brief Create a \c Simulator object.
    ///
    /// This member function creates and returns a pointer to a \c Simulator
    /// object, which represents the simulation engine configured with the
    /// specified parameters. The \c Simulator object is responsible for
    /// executing the simulation.
    ///
    /// \return A pointer to the created Simulator object.
    Simulator *createSimulator();

private:
    SimulatorType  m_Type;
    SimulationMode m_Mode;
    uint32_t       m_Cores              = 0UL;
    uint32_t       m_CheckpointInterval = 0UL;
    uint32_t       m_BatchSize          = 64UL;
    bool           m_CoreBinding        = false;
    uint32_t       m_GvtPeriod          = 1000UL;
};

} // namespace ispd::sim

#endif // ENGINE_SIMULATOR_HPP
