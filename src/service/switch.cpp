#include <algorithm>
#include <service/switch.hpp>

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
    m_Metrics.m_CommMBits += commSize;
    m_Metrics.m_CommTime  += commTime;
    m_Metrics.m_CommTasks++;

    const auto &routeDescriptor = event->getRouteDescriptor();

    /// Prepare the event to be sent to the next service.
    Event e(event->getTask(),
            RouteDescriptor(routeDescriptor.getSource(),
                            routeDescriptor.getDestination(),
                            getId(),
                            routeDescriptor.getOffset(),
                            routeDescriptor.getForwardingDirection()));

    /// Send the event to the next service.
    ispd::schedule_event(routeDescriptor.getPreviousService(),
                         departureTime,
                         TASK_ARRIVAL,
                         &e,
                         sizeof(e));
}
