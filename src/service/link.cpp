#include <algorithm>
#include <service/link.hpp>

void Link::onTaskArrival(timestamp_t now, const Task *t)
{
    const double commSize = t->getCommunicationSize();
    const double commTime = timeToCommunicate(commSize);

    const timestamp_t waitingTime   = std::max(0.0, m_AvailableTime - now);
    const timestamp_t departureTime = now + waitingTime + commTime;

    m_AvailableTime       = departureTime;
    m_Metrics.m_CommMBits += commSize;
    m_Metrics.m_CommTime  += commTime;
    m_Metrics.m_CommTasks++;

    m_Lvt = departureTime;

    /* Prepare the event */
    Event e(Task(t->getProcessingSize(), t->getCommunicationSize()));

    /* Send the event to the destination machine */
    schedule_event(m_To, departureTime, MACHINE_TASK_ARRIVAL, &e, sizeof(e));
}
