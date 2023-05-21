#include <algorithm>
#include <routing/table.hpp>
#include <service/machine.hpp>

extern RoutingTable *g_RoutingTable;

ENGINE_INLINE
static void doMachinePacketForwarding(const timestamp_t time,
                                      const Event      *event)
{
    const auto &routeDescriptor = event->getRouteDescriptor();

    const auto source      = routeDescriptor.getSource();
    const auto destination = routeDescriptor.getDestination();
    const auto offset      = routeDescriptor.getOffset();

    // It fetches the routing from the routing table using the source
    // and destination identifier.
    const Route *route = g_RoutingTable->getRoute(source, destination);

    // Prepare the event to be send to the next service.
    Event e(event->getTask(),
            RouteDescriptor(source, destination, offset + 1ULL));

    schedule_event((*route)[offset], time, TASK_ARRIVAL, &e, sizeof(e));
}

void Machine::onTaskArrival(const timestamp_t time, const Event *event)
{
    // It checks if the packet destination is not equals to this machine.
    // Therefore, the packet should be forwarded by the machine to the next
    // service in the route.
    if (event->getRouteDescriptor().getDestination() != getId()) {
        doMachinePacketForwarding(time, event);
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
