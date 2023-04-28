#include <iostream>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <scheduler/round_robin.hpp>

extern "C" {
#include <stdio.h>
#include <ROOT-Sim.h>
#ifndef NUM_LPS
#    define NUM_LPS 8
#endif

#ifndef NUM_TASKS
#    define NUM_TASKS 50
#endif

#ifndef NUM_THREADS
#    define NUM_THREADS 0
#endif
}


void *operator new(size_t s)
{
    return rs_malloc(s);
}

void operator delete(void *p) noexcept
{
    rs_free(p);
}

void operator delete(void *p, std::size_t) noexcept
{
    rs_free(p);
}

void *operator new[](std::size_t s)
{
    return rs_malloc(s);
}

void operator delete[](void *p) noexcept
{
    rs_free(p);
}

void operator delete[](void *p, size_t) noexcept
{
    rs_free(p);
}


void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s)
{
    switch (event_type)
    {
        case LP_FINI:
        {
            if (me == 0) {
                Machine *m = (Machine *)s;
                std::cout << "Machine Metrics" << std::endl;
                std::cout << "  LVT.................: " << m->getLocalVirtualTime() << " @ LP (" << me << ")" << std::endl;
                std::cout << "  Processed Mega-flops: " << m->getMetrics().m_ProcMFlops << " @ LP (" << me << ")" << std::endl;
                std::cout << "  Processed Time......: " << m->getMetrics().m_ProcTime << " @ LP (" << me << ")" << std::endl;
                std::cout << "  Processed Tasks.....: " << m->getMetrics().m_ProcTasks << " @ LP (" << me << ")" << std::endl;
                std::cout << std::endl;
            }
            break;
        }
        case LP_INIT:
        {
            if (me == 0) {
                Machine *m = new Machine(me, 2.0, 0.0, 2);
                SetState(m);
            } else {
                Master *m = new Master(me, new RoundRobin<sid_t>());
                SetState(m);

                m->getScheduler()->addResource(0);

                timestamp_t jitter = 0.0;
                for (int i = 0; i < 100; i++) {
                    Event e(Task(1.0 + i, 50.0));

                    schedule_event(me, jitter, MASTER_TASK_SCHEDULE, &e, sizeof(e));
                    jitter += 1e-52;
                }
            }
            break;
        }
        case MACHINE_TASK_ARRIVAL: {
            Machine *m = (Machine *)s;
            Event *e = (Event *)content;
            m->onTaskArrival(now, &e->getTask());
            break;
        }
        case MASTER_TASK_SCHEDULE: {
            Master *m = (Master *)s;
            Event *e = (Event *)content;
            m->onTaskSchedule(now, &e->getTask());
            break;
        }
        default:
            fprintf(stderr, "Unknown event type\n");
            abort();
    }
}

bool CanEnd(lp_id_t me, const void *snapshot)
{
    return false;
}


struct simulation_configuration conf = {
        .lps              = NUM_LPS,
        .n_threads        = NUM_THREADS,
        .termination_time = 0,
        .gvt_period       = 1000,
        .log_level        = LOG_INFO,
        .stats_file       = "phold",
        .ckpt_interval    = 0,
        .prng_seed        = 0,
        .core_binding     = false,
        .serial           = false,
        .dispatcher       = ProcessEvent,
        .committed        = CanEnd,
};

int main(int argc, char **argv)
{
    --argc, ++argv;

    RootsimInit(&conf);
    return RootsimRun();
}
