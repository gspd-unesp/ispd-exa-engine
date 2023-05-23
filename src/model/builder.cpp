#include <allocator/rootsim_allocator.hpp>
#include <model/builder.hpp>
#include <service/machine.hpp>

void ispd::model::Builder::registerMachine(const sid_t  serviceId,
                                           const double power,
                                           const double loadFactor,
                                           const int    cores)
{
    // It checks if the power specified is non-positive. If so,
    // the program will be immediately aborted.
    if (UNLIKELY(power <= 0.0))
        die("Registering the machine %llu we encountered that the power is "
            "non-positive (%lf).",
            serviceId,
            power);

    // It checks if the load factor is out of the interval [0, 1]. If so,
    // the program will be immediately aborted.
    if (UNLIKELY(loadFactor < 0.0 || loadFactor > 1.0))
        die("Registering the machine %llu we encountered that the load factor "
            "(%lf) is out of the interval [0, 1].",
            serviceId,
            loadFactor);

    // An initializer service is registered for the machine with the specified
    // parameters.
    m_Simulator->registerService(
        serviceId, [serviceId, power, loadFactor, cores]() {
            Machine *m = ROOTSimAllocator<>::construct<Machine>(
                serviceId, power, loadFactor, cores);
            return m;
        });
}
