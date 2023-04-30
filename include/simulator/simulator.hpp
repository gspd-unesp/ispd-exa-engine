#ifndef ENGINE_SIMULATOR_HPP
#define ENGINE_SIMULATOR_HPP

#include <core/core.hpp>
#include <functional>
#include <service/service.hpp>
#include <unordered_map>

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
     *        when the model is initialized to be run.
     *
     * @param service the service
     */
    ENGINE_INLINE void registerService(const sid_t serviceId, const std::function<Service *()> &serviceInitializer)
    {
        // It checks if a service with that id has already been registered. If so,
        // then the program is immediately aborted.
        if (m_ServiceInitializers.find(serviceId) != m_ServiceInitializers.end())
            die("A service with id %lu has already been registered.", serviceId);

        // Register the service.
        m_ServiceInitializers.insert(std::make_pair(serviceId, serviceInitializer));
    }

    /**
     * @brief It executes the simulation.
     */
    virtual void simulate() = 0;

    /**
     * @brief Returns a const (read-only) reference to the service initializer.
     *
     * @return a const (read-only) reference to the service initializers
     */
    ENGINE_INLINE const std::unordered_map<sid_t, std::function<Service *()>> &getServices()
    {
        return m_ServiceInitializers;
    }

protected:
    /**
     * @brief It contains the services that will be simulated.
     */
    std::unordered_map<sid_t, std::function<Service *()>> m_ServiceInitializers{};
};

#endif // ENGINE_SIMULATOR_HPP
