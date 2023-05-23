#include "core/core.hpp"
#include <allocator/rootsim_allocator.hpp>
#include <customer/customer.hpp>
#include <math/utility.hpp>
#include <model/builder.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>

void ispd::model::Builder::registerMaster(
    const sid_t                     masterId,
    MasterScheduler                 schedulerType,
    std::function<void(Master *)> &&callback)
{
    m_Simulator->registerService(
        masterId, [masterId, schedulerType, &callback]() {
            Scheduler<sid_t> *scheduler = nullptr;

            switch (schedulerType) {
            case MasterScheduler::ROUND_ROBIN:
                scheduler = ROOTSimAllocator<>::construct<RoundRobin<sid_t>>();
                break;
            default:
                die("Registering the master %llu we encountered that the "
                    "scheduler type is invalid (%u).",
                    masterId,
                    schedulerType);
            }

            Master *m =
                ROOTSimAllocator<>::construct<Master>(masterId, scheduler);
            callback(m);
            return m;
        });
}

void ispd::model::Builder::registerMachine(const sid_t  machineId,
                                           const double power,
                                           const double loadFactor,
                                           const int    cores)
{
    // It checks if the power specified is non-positive. If so,
    // the program will be immediately aborted.
    if (UNLIKELY(power <= 0.0))
        die("Registering the machine %llu we encountered that the power is "
            "non-positive (%lf).",
            machineId,
            power);

    // It checks if the load factor is out of the interval [0, 1]. If so,
    // the program will be immediately aborted.
    if (UNLIKELY(loadFactor < 0.0 || loadFactor > 1.0))
        die("Registering the machine %llu we encountered that the load factor "
            "(%lf) is out of the interval [0, 1].",
            machineId,
            loadFactor);

    m_Simulator->registerService(
        machineId, [machineId, power, loadFactor, cores]() {
            return ROOTSimAllocator<>::construct<Machine>(
                machineId, power, loadFactor, cores);
        });
}
void ispd::model::Builder::registerLink(const sid_t  linkId,
                                        const sid_t  from,
                                        const sid_t  to,
                                        const double bandwidth,
                                        const double loadFactor,
                                        const double latency)
{

    // It checks if the load factor is out of the interval [0, 1]. If so,
    // the program will be immediately aborted.
    if (UNLIKELY(loadFactor < 0.0 || loadFactor > 1.0))
        die("Registering the link %llu we encountered that the load factor "
            "(%lf) is out of the interval [0, 1].",
            linkId,
            loadFactor);

    m_Simulator->registerService(
        linkId, [linkId, from, to, bandwidth, loadFactor, latency]() {
            return ROOTSimAllocator<>::construct<Link>(
                linkId, from, to, bandwidth, loadFactor, latency);
        });
}

#include <iostream>
void ispd::model::workload::zeroth::addConstantSizedWorkload(
    const sid_t    masterId,
    const double   processingSize,
    const double   communicationSize,
    const uint32_t taskAmount,
    const bool     jittered)
{
    if (jittered) {
        timestamp_t arrivalTime = 0.0;

        for (uint32_t i = 0; i < taskAmount; i++) {
            // It calculates a unique task identifier using the Szudzik's
            // pairing function, taking the index of the task being generated
            // and the master's identifier as arguments to generate the unique
            // identifier.
            const uint64_t taskId = szudzik(i, masterId);

            // Prepare the event.
            Event e(Task(taskId, processingSize, communicationSize));

            // Send the event.
            ispd::schedule_event(masterId, arrivalTime, TASK_ARRIVAL, &e, sizeof(e));

            arrivalTime += 1e-52;
        }
    }
    else {
        for (uint32_t i = 0; i < taskAmount; i++) {
            // It calculates a unique task identifier using the Szudzik's
            // pairing function, taking the index of the task being generated
            // and the master's identifier as arguments to generate the unique
            // identifier.
            const uint64_t taskId = szudzik(i, masterId);

            // Prepare the event.
            Event e(Task(taskId, processingSize, communicationSize));

            // Send the event.
            ispd::schedule_event(masterId, 0.0, TASK_ARRIVAL, &e, sizeof(e));
        }
    }
}
