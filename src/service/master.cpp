#include <routing/table.hpp>
#include <service/master.hpp>

extern RoutingTable *g_RoutingTable;

void Master::onTaskArrival(timestamp_t time, const Event *event)
{
    /* Schedule the slave which will receive the task */
    sid_t scheduledSlave = m_Scheduler->schedule();

    /* Prepare the event */
    Event e(event->getTask(),
            RouteDescriptor(getId(),
                            scheduledSlave,
                            getId(),
                            event->getRouteDescriptor().getOffset() + 1ULL));

    const Route *route = g_RoutingTable->getRoute(getId(), scheduledSlave);

    /* Schedule the event to the scheduled slave */
    ispd::schedule_event((*route)[0], time, TASK_ARRIVAL, &e, sizeof(e));
}
