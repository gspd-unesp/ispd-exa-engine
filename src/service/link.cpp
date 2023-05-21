#include <algorithm>
#include <service/link.hpp>

void Link::onTaskArrival(timestamp_t now, const Event *event)
{
    const Task  &task     = event->getTask();
    const double commSize = task.getCommunicationSize();
    const double commTime = timeToCommunicate(commSize);

    const timestamp_t waitingTime   = std::max(0.0, m_AvailableTime - now);
    const timestamp_t departureTime = now + waitingTime + commTime;

    m_AvailableTime        = departureTime;
    m_Metrics.m_CommMBits += commSize;
    m_Metrics.m_CommTime  += commTime;
    m_Metrics.m_CommTasks++;

    m_Lvt = departureTime;

    const auto &routeDescriptor = event->getRouteDescriptor();

    /* Prepare the event */
    Event e(event->getTask(),
            RouteDescriptor(routeDescriptor.getSource(),
                            routeDescriptor.getDestination(),
                            getId(),
                            routeDescriptor.getOffset()));

    sid_t sendTo;

    if (m_From == routeDescriptor.getPreviousService())
        sendTo = m_To;
    else if (m_To == routeDescriptor.getPreviousService())
        sendTo = m_From;
    else
        die("Link with id %llu has received a packet from a service different "
            "from its ends.",
            getId());

    /* Send the event to the destination machine */
    schedule_event(sendTo, departureTime, TASK_ARRIVAL, &e, sizeof(e));
}
