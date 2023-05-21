#include <algorithm>
#include <routing/table.hpp>
#include <service/machine.hpp>

extern RoutingTable *g_RoutingTable;

void Machine::onTaskArrival(const timestamp_t time, const Event *event)
{
    if (event->getDestination() != getId()) {
        const sid_t       source      = event->getSource();
        const sid_t       destination = event->getDestination();
        const std::size_t offset      = event->getOffset();

        const Route *route = g_RoutingTable->getRoute(source, destination);

        // Prepare the event to be send to the next service.
        Event e(event->getTask(), source, destination, offset + 1ULL);

        schedule_event((*route)[offset], time, TASK_ARRIVAL, &e, sizeof(e));
        return;
    }

    const Task  &task     = event->getTask();
    const double procSize = task.getProcessingSize();
    const double procTime = timeToProcess(procSize);

    m_Metrics.m_ProcMFlops += procSize;
    m_Metrics.m_ProcTime   += procTime;
    m_Metrics.m_ProcTasks++;

    int               coreIndex;
    const timestamp_t leastCoreTime = timeToAttend(&coreIndex);
    const timestamp_t waitingTime   = std::max(0.0, leastCoreTime - time);
    const timestamp_t departureTime = time + waitingTime + procTime;

    m_CoreFreeTimes[coreIndex] = departureTime;
    m_LVT                      = departureTime;
}
