//
// Created by willao on 20/06/23.
//

#include <service/switch.hpp>

void Switch::onTaskArrival(timestamp_t now, const Event *event)
{
    const Task &task = event->getTask()
    const double communication_size = task->getCommunicationSize();
    const timestamp_t timewait = std::max(0.0, sw_AvaliableTime - now);
    const timestamp_t departureTime = now + timewait + event->getTask()->get
    switchMetrics.sw_CommMBits += communication_size;
    switchMetrics.sw_CommTime += timeToCommunicate(communication_size);
    switchMetrics.sw_CommTasks++;
    const auto &routeDescriptor = event->getRouteDescriptor();

    /* prepare the event */
    Event e(event->getTask(), RouteDescriptor(routeDescriptor.getSource(),
                                              routeDescriptor.getDestination(),
                                              getId(),
                                              routeDescriptor.getOffset(),
                                              routeDescriptor.getFowardingDirection()));
    ispd::schedule_event(routeDescriptor.getPreviousService(),
                         departureTime)
}
