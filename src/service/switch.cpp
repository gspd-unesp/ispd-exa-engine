#include <algorithm>
#include <core/core.hpp>
#include <routing/table.hpp>
#include <service/switch.hpp>

extern RoutingTable *g_RoutingTable;

ENGINE_INLINE
static void doSwitchPacketForwarding(const sid_t       switchId,
                                     const timestamp_t now,
                                     const Event      *event)
{
    const auto &routeDescriptor = event->getRouteDescriptor();

    const auto source           = routeDescriptor.getSource();
    const auto destination      = routeDescriptor.getDestination();
    const auto offset           = routeDescriptor.getOffset();
    const auto forwardDirection = routeDescriptor.getForwardingDirection();
    const auto newOffset = forwardDirection ? offset + 1ULL : offset - 1ULL;

    // It fetches the routing from the routing table using the source
    // and destination identifier.
    const Route *route = g_RoutingTable->getRoute(source, destination);

    // Prepare the event to be send to the next service.
    Event e(event->getTask(),
            RouteDescriptor(
                source, destination, switchId, newOffset, forwardDirection));

    ispd::schedule_event((*route)[offset], now, TASK_ARRIVAL, &e, sizeof(e));
}

void Switch::onTaskArrival(timestamp_t now, const Event *event)
{
    const Task &task = event->getTask();

    /// Calculate the necessary communication time based on the
    /// communication size of the task.
    const double commSize = task.getCommunicationSize();
    const double commTime = timeToCommunicate(commSize);

    /// Calculate the internal queueing model timings.
    const timestamp_t waitingTime   = std::max(0.0, m_AvailableTime - now);
    const timestamp_t departureTime = now + waitingTime + commTime;

    m_AvailableTime = departureTime;

    /// Update the switch metrics.
    m_Metrics.m_LastActivityTime  = departureTime;
    m_Metrics.m_CommMBits        += commSize;
    m_Metrics.m_CommTime         += commTime;
    m_Metrics.m_CommPackets++;

    /// Forward the packet.
    doSwitchPacketForwarding(getId(), now, event);
}
