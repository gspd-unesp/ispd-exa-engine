#include <service/master.hpp>

void Master::onTaskSchedule(const timestamp_t time, const Task *t)
{
    /* Schedule the slave which will receive the task */
    sid_t scheduledSlave = m_Scheduler->schedule();

    /* Prepare the event */
    Event e(Task(t->getProcessingSize(), t->getCommunicationSize()));

    /* Schedule the event to the scheduled slave */
    schedule_event(scheduledSlave, time, MACHINE_TASK_ARRIVAL, &e, sizeof(e));
}
