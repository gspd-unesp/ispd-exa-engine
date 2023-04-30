#include <engine.hpp>
#include <iostream>
#include <mutex>
#include <service/machine.hpp>
#include <simulator/timewarp.hpp>

static TimeWarpSimulator          *tw;
static bool                        enabledROOTSimAllocation = false;
ENGINE_TEMPORARY static std::mutex print_mutex;

#ifdef ROOTSIM_ENGINE
void *operator new(size_t s)
{
    if (LIKELY(enabledROOTSimAllocation))
        return rs_malloc(s);
    else
        return std::malloc(s);
}

void operator delete(void *p) noexcept
{
    if (LIKELY(enabledROOTSimAllocation))
        rs_free(p);
    else
        return std::free(p);
}

void operator delete(void *p, std::size_t) noexcept
{
    if (LIKELY(enabledROOTSimAllocation))
        rs_free(p);
    else
        std::free(p);
}

void *operator new[](std::size_t s)
{
    if (LIKELY(enabledROOTSimAllocation))
        return rs_malloc(s);
    else
        return std::malloc(s);
}

void operator delete[](void *p) noexcept
{
    if (LIKELY(enabledROOTSimAllocation))
        rs_free(p);
    else
        std::free(p);
}

void operator delete[](void *p, size_t) noexcept
{
    if (LIKELY(enabledROOTSimAllocation))
        rs_free(p);
    else
        std::free(p);
}
#endif // ROOTSIM_ENGINE

/**
 * ROOT-Sim's simulation configuration.
 */
static struct simulation_configuration conf = {
    .lps              = 0,
    .n_threads        = 0 /* 0 - all threads */,
    .termination_time = 0,
    .gvt_period       = 1000,
    .log_level        = LOG_INFO,
    .stats_file       = "phold",
    .ckpt_interval    = 0,
    .prng_seed        = 0,
    .core_binding     = false,
    .serial           = false,
    .dispatcher       = nullptr,
    .committed        = nullptr,
};

void TimeWarpSimulator::simulate()
{
    tw                       = this;
    enabledROOTSimAllocation = true;

    /* Update the ROOT-Sim's simulation configuration */
    conf.lps        = m_ServiceInitializers.size();
    conf.committed  = [](lp_id_t me, const void *snapshot) { return false; };
    conf.dispatcher = [](lp_id_t me, simtime_t now, unsigned event_type, const void *content, unsigned size, void *s) {
        switch (event_type) {
        case LP_FINI: {
            DEBUG_BLOCK({
                if ((me % 2) == 1) {
                    print_mutex.lock();
                    Machine *m = (Machine *)s;
                    std::cout << "\nMachine Metrics" << std::endl;
                    std::cout << " - LVT................: " << m->getLocalVirtualTime() << " @ LP (" << me << ")"
                              << std::endl;
                    std::cout << " - Processed Megaflops: " << m->getMetrics().m_ProcMFlops << " @ LP (" << me << ")"
                              << std::endl;
                    std::cout << " - Processed Time.....: " << m->getMetrics().m_ProcTime << " @ LP (" << me << ")"
                              << std::endl;
                    std::cout << " - Processed Tasks....: " << m->getMetrics().m_ProcTasks << " @ LP (" << me << ")"
                              << std::endl;
                    print_mutex.unlock();
                }
            });
            break;
        }
        case LP_INIT: {
            // It checks if no service has been registered with that id.
            if (UNLIKELY(tw->getServices().find(me) == tw->getServices().end()))
                die("Service with id %llu has not been found.", me);

            const std::function<Service *()> &serviceInitializer = tw->getServices().at(me);

            Service *service = serviceInitializer();
            SetState(service);
            break;
        }
        case TASK_ARRIVAL: {
            /* This service may be a machine, link, master etc. */
            Service *service = (Service *)s;
            Event   *e       = (Event *)content;

            /* Calls the service's task arrival handler */
            service->onTaskArrival(now, &e->getTask());
            break;
        }
        default:
            fprintf(stderr, "Unknown event type\n");
            abort();
        }
    };

    /* Initialize the ROOT-Sim */
    RootsimInit(&conf);

    /* Run the ROOT-Sim */
    RootsimRun();
}
