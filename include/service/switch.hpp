#ifndef ISPD_EXA_ENGINE_SWITCH_HPP
#define ISPD_EXA_ENGINE_SWITCH_HPP

#include <service/link.hpp>
#include <service/service.hpp>

struct SwitchMetrics
{
    double m_CommMBits;
    double m_CommTime;
    double m_CommTasks;
};

/**
 * Switch is responsible for fowarding messages to other
 * machines through the links
 */
class Switch : public Service
{
public:
    /**
     * Constructor which specifies the switch id, bandwidth (megatibs, amount of
     * latency and its load factor
     * @param id        switch's identifier
     * @param bandwidth bandwidth in megabits
     * @param latency   latency in seconds
     * @param load_factor load factor
     */
    explicit Switch(const sid_t  id,
                    const double bandwidth,
                    const double latency,
                    const double load_factor)
        : Service(id), m_Bandwidth(bandwidth), m_Latency(latency),
          m_LoadFactor(load_factor), m_AvailableTime(0.0)
    {}

    /**
     * It calculates the time taken in seconds to a switch communicate a
     * customer
     * @param communication_size communication size in megabits
     * @return the time taken in seconds
     */
    ENGINE_INLINE double timeToCommunicate(const double communicationSize) const
    {
        return m_Latency +
               communicationSize / ((1.0 - m_LoadFactor) * m_Bandwidth);
    }

    void onTaskArrival(timestamp_t, const Event *event) override;

private:
    SwitchMetrics m_Metrics{};
    double        m_Bandwidth;
    double        m_Latency;
    double        m_LoadFactor;
    timestamp_t   m_AvailableTime;
};

#endif // ISPD_EXA_ENGINE_SWITCH_HPP
