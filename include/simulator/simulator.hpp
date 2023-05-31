#ifndef ENGINE_SIMULATOR_HPP
#define ENGINE_SIMULATOR_HPP

#include <core/core.hpp>
#include <functional>
#include <memory>
#include <service/service.hpp>
#include <unordered_map>

namespace ispd::sim
{

/// \brief Simulation mode enumeration.
///
/// \details The simulation mode is used to select which simulator type will be
///          used to progress the simulation, such that, each mode changes how
///          the simulator will work internally to progress the simulation.
enum class SimulationMode
{
    SEQUENTIAL,
    OPTIMISTIC,
    CONSERVATIVE
};

/// \brief Simulator type.
///
/// \details It specifies which underlying simulator is being used
///          to progress the simulation.
enum class SimulatorType
{
    ROOTSIM
};

/**
 * @brief Base simulator class.
 */
class Simulator
{
public:
    /**
     * @brief It register a service initializer.
     *
     *        The service initializer is a piece of code that is run
     *        when the service is initialized by the simulation.
     *
     * @param service the service
     * @param serviceInitializer the service initializer
     */
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

    /**
     * @brief It register a service finalizer.
     *
     *        The service initializer is a piece of code that is run
     *        when the service is finalized by the simulation.
     *
     * @param service the service
     * @param serviceFinalizer the service finalizer
     */
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

    /**
     * @brief It executes the simulation.
     */
    virtual void simulate() = 0;

    /**
     * @brief Returns a const (read-only) reference to the services
     * initializers.
     *
     * @return a const (read-only) reference to the services initializers
     */
    ENGINE_INLINE const std::unordered_map<sid_t, std::function<Service *()>>                     &
    getServices()
    {
        return m_ServiceInitializers;
    }

    /**
     * @brief Returns a const (read-only) reference to the services finalizers.
     *
     * @return a const (read-only) reference to the services finalizers
     */
    ENGINE_INLINE const std::unordered_map<sid_t,
                                           std::function<void(Service *)>>                     &
    getServicesFinalizers()
    {
        return m_ServiceFinalizers;
    }

protected:
    /**
     * @brief It contains code sections that will be called when a service
     *        with the respective identifier be initialized.
     */
    std::unordered_map<sid_t, std::function<Service *()>>
        m_ServiceInitializers{};

    /**
     * @brief It contains code sections that will be called when a service
     *        with the respective identifier be finalized.
     */
    std::unordered_map<sid_t, std::function<void(Service *)>>
        m_ServiceFinalizers{};
};

class SimulatorBuilder
{
public:
    explicit SimulatorBuilder(const SimulatorType  type,
                              const SimulationMode mode)
        : m_Type(type), m_Mode(mode)
    {}

    SimulatorBuilder &cores(const uint32_t cores);
    SimulatorBuilder &checkpointInterval(const uint32_t interval);
    SimulatorBuilder &enableCoreBinding();
    SimulatorBuilder &gvtPeriod(const uint32_t period);
    Simulator        *createSimulator();

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
