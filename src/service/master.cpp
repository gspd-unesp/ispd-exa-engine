#include <customer/customer.hpp>
#include <routing/table.hpp>
#include <service/master.hpp>

extern RoutingTable *g_RoutingTable;

void Master::onSchedulerInit(timestamp_t now)
{
    m_Scheduler->onInit();
}

void Master::onTaskArrival(timestamp_t time, const Event *event)
{
    m_Metrics.m_LastActivityTime = time;

    if (event->getTask().getCompletionState() ==
        TaskCompletionState::PROCESSED) {

        /// Update the master's metrics upon task completion.
        m_Metrics.m_CompletedTasks++;

        // In this case, we have a processed task in which its origin
        // is exactly this master. Therefore, a new task is tried to be
        // scheduled if there are remaining tasks.
        if (event->getTask().getOrigin() == getId()) {

            const Task            &task = event->getTask();
            const RouteDescriptor &routeDescriptor =
                event->getRouteDescriptor();
            const sid_t slaveId = routeDescriptor.getDestination();

            m_Scheduler->onCompletedTask(time, slaveId, task);
            return;
        }
        // In this case, we have a processed task in which its origin is
        // not this master. However, this master has contributed in the
        // task scheduling when it has previously arrived in this master.
        //
        // Further, this master should forward this packet to the master
        // whcih that is the origin of the task. However, the destination
        // of the route descriptor is changed for this master's identifier.
        else {
            const auto &routeDescriptor = event->getRouteDescriptor();
            const auto  offset          = routeDescriptor.getOffset();
            const auto  forwardingDirection =
                routeDescriptor.getForwardingDirection();
            const auto newOffset =
                forwardingDirection ? offset + 1ULL : offset - 1ULL;

            /* Prepare the event */
            Event e(event->getTask(),
                    RouteDescriptor(event->getTask().getOrigin(),
                                    getId(),
                                    getId(),
                                    newOffset,
                                    false));

            const Route *route =
                g_RoutingTable->getRoute(event->getTask().getOrigin(), getId());

            /* Schedule the event to the scheduled slave */
            ispd::schedule_event(
                (*route)[offset], time, TASK_ARRIVAL, &e, sizeof(e));
            return;
        }
    }

    /* Schedule the slave which will receive the task */
    sid_t scheduledSlave = m_Scheduler->schedule();

    /* Prepare the event */
    Event e(event->getTask(),
            RouteDescriptor(getId(), scheduledSlave, getId(), 1ULL, true));

    const Route *route = g_RoutingTable->getRoute(getId(), scheduledSlave);

    /* Schedule the event to the scheduled slave */
    ispd::schedule_event((*route)[0], time, TASK_ARRIVAL, &e, sizeof(e));
}
