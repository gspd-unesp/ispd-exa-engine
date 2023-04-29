#include <iostream>
#include <mutex>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>

extern "C"
{
#include <ROOT-Sim.h>
#include <stdio.h>
#ifndef NUM_LPS
#    define NUM_LPS 2
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

std::mutex print_mutex;

void ProcessEvent(lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s)
{
    switch (event_type) {
    case LP_FINI: {
        if (me == 0) {
            print_mutex.lock();
            Machine *m = (Machine *)s;
            std::cout << "Machine Metrics" << std::endl;
            std::cout << "  LVT................: " << m->getLocalVirtualTime() << " @ LP (" << me << ")" << std::endl;
            std::cout << "  Processed Megaflops: " << m->getMetrics().m_ProcMFlops << " @ LP (" << me << ")"
                      << std::endl;
            std::cout << "  Processed Time.....: " << m->getMetrics().m_ProcTime << " @ LP (" << me << ")" << std::endl;
            std::cout << "  Processed Tasks....: " << m->getMetrics().m_ProcTasks << " @ LP (" << me << ")"
                      << std::endl;
            std::cout << std::endl;
            print_mutex.unlock();
        }
        else if (me == 1) {
            print_mutex.lock();
            Link *l = (Link *)s;
            std::cout << "Link Metrics" << std::endl;
            std::cout << "  LVT..................: " << l->getLocalVirtualTime() << " @ LP (" << me << ")" << std::endl;
            std::cout << "  Communicated Megabits: " << l->getMetrics().m_CommMBits << " @ LP (" << me << ")"
                      << std::endl;
            std::cout << "  Communicated Time....: " << l->getMetrics().m_CommTime << " @ LP (" << me << ")"
                      << std::endl;
            std::cout << "  Communicated Tasks...: " << l->getMetrics().m_CommTasks << " @ LP (" << me << ")"
                      << std::endl;
            std::cout << std::endl;
            print_mutex.unlock();
        }
        break;
    }
    case LP_INIT: {
        if (me == 0) {
            Machine *m = new Machine(me, 2.0, 0.0, 2);
            SetState(m);
        }
        else {
            Link *l = new Link(me, static_cast<sid_t>(-1.0), 0.0, 5.0, 0.0, 1.0);
            SetState(l);

            Event e1(Task(10, 50));
            Event e2(Task(20, 80));

            schedule_event(l->getId(), 0.0, LINK_TASK_ARRIVAL, &e1, sizeof(e1));
            schedule_event(l->getId(), 15.0, LINK_TASK_ARRIVAL, &e2, sizeof(e2));
        }
        break;
    }
    case MACHINE_TASK_ARRIVAL: {
        Machine *m = (Machine *)s;
        Event   *e = (Event *)content;
        m->onTaskArrival(now, &e->getTask());
        break;
    }
    case LINK_TASK_ARRIVAL: {
        Link  *l = (Link *)s;
        Event *e = (Event *)content;
        l->onTaskArrival(now, &e->getTask());
        break;
    }
    case MASTER_TASK_SCHEDULE: {
        Master *m = (Master *)s;
        Event  *e = (Event *)content;
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
