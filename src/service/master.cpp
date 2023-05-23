#include "customer/customer.hpp"
#include <routing/table.hpp>
#include <service/master.hpp>

extern RoutingTable *g_RoutingTable;

void Master::onTaskArrival(timestamp_t time, const Event *event)
{
    m_Metrics.m_LastActivityTime = time;

    if (event->getTask().getCompletionState() ==
        TaskCompletionState::PROCESSED) {
        m_Metrics.m_CompletedTasks++;
        return;
    }

    const auto &routeDescriptor     = event->getRouteDescriptor();
    const auto  offset              = routeDescriptor.getOffset();
    const auto  forwardingDirection = routeDescriptor.getForwardingDirection();
    const auto  newOffset = forwardingDirection ? offset + 1ULL : offset - 1ULL;

    /* Schedule the slave which will receive the task */
    sid_t scheduledSlave = m_Scheduler->schedule();

    /* Prepare the event */
    Event e(
        event->getTask(),
        RouteDescriptor(
            getId(), scheduledSlave, getId(), newOffset, forwardingDirection));

    const Route *route = g_RoutingTable->getRoute(getId(), scheduledSlave);

    /* Schedule the event to the scheduled slave */
    ispd::schedule_event((*route)[0], time, TASK_ARRIVAL, &e, sizeof(e));
}
