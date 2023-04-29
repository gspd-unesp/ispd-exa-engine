#include <algorithm>
#include <service/machine.hpp>

void Machine::onTaskArrival(const timestamp_t time, const Task *t)
{
    const double procSize = t->getProcessingSize();
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
