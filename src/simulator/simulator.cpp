#include <simulator/rootsim.hpp>
#include <simulator/simulator.hpp>

using namespace ispd::sim;

SimulatorBuilder &SimulatorBuilder::cores(const uint32_t cores)
{
    m_Cores = m_Mode == SimulationMode::SEQUENTIAL ? 1UL : cores;
    return *this;
}

SimulatorBuilder &SimulatorBuilder::checkpointInterval(const uint32_t interval)
{
    m_CheckpointInterval = interval;
    return *this;
}

SimulatorBuilder &SimulatorBuilder::enableCoreBinding()
{
    m_CoreBinding = true;
    return *this;
}

SimulatorBuilder &SimulatorBuilder::gvtPeriod(const uint32_t period)
{
    m_GvtPeriod = period;
    return *this;
}

Simulator *SimulatorBuilder::createSimulator()
{
    switch (m_Type) {
    case SimulatorType::ROOTSIM: {
        // It checks if the user has selected the ROOT-Sim underlying simulator
        // and the conservative simulation mode. With that, since the ROOT-Sim
        // does not implement the conservative synchronization protocol, this
        // cannot be done and, therefore, the program will be immediately
        // aborted.
        if (m_Mode == SimulationMode::CONSERVATIVE)
            die("ROOT-Sim does not implement the conservative synchronization "
                "protocol.");

        struct simulation_configuration conf = {
            .n_threads        = m_Cores,
            .termination_time = 0,
            .gvt_period       = m_GvtPeriod,
            .log_level  = LOG_INFO, // @Temporary: This will be removed later.
            .stats_file = "phold",  // @Temporary: This will be removed later.
            .ckpt_interval = m_CheckpointInterval,
            .prng_seed     = 0,
            .core_binding  = m_CoreBinding,
            .serial        = m_Mode == SimulationMode::SEQUENTIAL,
        };

        switch (m_Mode) {
        case SimulationMode::SEQUENTIAL:
        case SimulationMode::OPTIMISTIC:
            return new ROOTSimSimulator(std::move(conf));
        default:
            die("Unknown simulation type (%lu).", m_Mode);
        }

        break;
    }
    default:
        die("Unknown simulator type (%lu).", m_Type);
    }
    return nullptr;
}
