#include <service/master.hpp>

void Master::onTaskArrival(timestamp_t time, const Task *t)
{
    /* Schedule the slave which will receive the task */
    sid_t scheduledSlave = m_Scheduler->schedule();

    /* Prepare the event */
    Event e(Task(t->getTid(), t->getProcessingSize(), t->getCommunicationSize()));

    /* Schedule the event to the scheduled slave */
    schedule_event(scheduledSlave, time, TASK_ARRIVAL, &e, sizeof(e));
}
