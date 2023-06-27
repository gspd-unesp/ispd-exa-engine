#include <math/utility.hpp>
#include <routing/table.hpp>
#include <scheduler/round_robin.hpp>
#include <service/master.hpp>

extern RoutingTable *g_RoutingTable;

void RoundRobin::onInit()
{
    Workload *workload = m_Master->getWorkload();

    /// If the master does not have workload and just schedules the tasks
    /// that it receives, then no initialization must be done in relation
    /// to scheduling initial tasks.
    if (!workload)
        return;

    double arrivalTime = 0.0;

    for (int i = 0; i < m_Resources.size(); i++) {
        if (UNLIKELY(!workload->hasRemainingWorkload()))
            return;

        const sid_t    masterId = m_Master->getId();
        const uint64_t taskId =
            szudzik(workload->getRemainingTasks(), masterId);

        double processingSize;
        double communicationSize;

        workload->setTaskWorkload(processingSize, communicationSize);
        workload->setTaskArrivalTime(arrivalTime);

        const sid_t  scheduledSlave = schedule();
        const Route *route = g_RoutingTable->getRoute(masterId, scheduledSlave);

        Event e(
            Task(taskId, masterId, processingSize, communicationSize),
            RouteDescriptor(masterId, scheduledSlave, masterId, 1ULL, true));

        /* Schedule the event to the scheduled slave */
        ispd::schedule_event(
            (*route)[0], arrivalTime, TASK_ARRIVAL, &e, sizeof(e));
    }
}

void RoundRobin::onCompletedTask(const timestamp_t now,
                                 const uint64_t    slaveId,
                                 const Task       &task)
{
    if (UNLIKELY(!m_Master->m_Workload->hasRemainingWorkload()))
        return;

    const sid_t    masterId = m_Master->getId();
    const uint64_t taskId =
        szudzik(m_Master->m_Workload->getRemainingTasks(), masterId);

    double processingSize;
    double communicationSize;
    double arrivalTime = now;

    m_Master->m_Workload->setTaskWorkload(processingSize, communicationSize);
    m_Master->m_Workload->setTaskArrivalTime(arrivalTime);

    const sid_t  scheduledSlave = schedule();
    const Route *route = g_RoutingTable->getRoute(masterId, scheduledSlave);

    Event e(Task(taskId, masterId, processingSize, communicationSize),
            RouteDescriptor(masterId, scheduledSlave, masterId, 1ULL, true));

    /* Schedule the event to the scheduled slave */
    ispd::schedule_event((*route)[0], arrivalTime, TASK_ARRIVAL, &e, sizeof(e));
}
