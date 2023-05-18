#include <engine.hpp>
#include <iostream>
#include <mutex>
#include <service/machine.hpp>
#include <simulator/timewarp.hpp>

static TimeWarpSimulator          *tw;
ENGINE_TEMPORARY static std::mutex print_mutex;

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
    tw = this;

    /* Update the ROOT-Sim's simulation configuration */
    conf.lps        = m_ServiceInitializers.size();
    conf.committed  = [](lp_id_t me, const void *snapshot) { return false; };
    conf.dispatcher = [](lp_id_t     me,
                         simtime_t   now,
                         unsigned    event_type,
                         const void *content,
                         unsigned    size,
                         void       *s) {
        switch (event_type) {
        case LP_FINI: {
            // It checks if no service finalizer has been registered for the current service.
            // Unlikely the service initializer, there is no strict requirement for all services
            // to have a service finalizer.
            if (UNLIKELY(tw->getServicesFinalizers().find(me) == tw->getServicesFinalizers().end()))
                return;

            const std::function<void (Service *)> &serviceFinalizer = tw->getServicesFinalizers().at(me);
            serviceFinalizer((Service *)s);
            break;
        }
        case LP_INIT: {
            // It checks if no service has been registered with that id.
            if (UNLIKELY(tw->getServices().find(me) == tw->getServices().end()))
                die("Service with id %llu has not been found.", me);

            const std::function<Service *()> &serviceInitializer =
                tw->getServices().at(me);

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
