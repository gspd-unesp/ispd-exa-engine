//
// Created by willao on 19/06/23.
//

#ifndef ISPD_EXA_ENGINE_SWITCH_HPP
#define ISPD_EXA_ENGINE_SWITCH_HPP
#include <service/link.hpp>
#include <service/service.hpp>
struct switchMetrics
{
    double sw_CommMBits;
    double sw_CommTime;
    double sw_CommTasks;
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
    explicit Switch(const sid_t id, const double bandwidth, const double latency,
                    const double load_factor)
        :Service(id), sw_Bandwidth(bandwidth), sw_Latency(latency), sw_loadFactor(load_factor)
          , sw_AvaliableTime(0.0)
    {}


    /**
     * It calculates the time taken in seconds to a switch communicate a customer
     * @param communication_size communication size in megabits
     * @return the time taken in seconds
     */
    ENGINE_INLINE double timeToCommunicate(const double communication_size) const
    {
        return sw_Latency + communication_size / ((1-sw_loadFactor) * sw_Bandwidth);
    }


    void onTaskArrival(timestamp_t, const Event *event) override;


private:
    switchMetrics           sw_Metrics{};
    double                  sw_Bandwidth;
    double                  sw_Latency;
    double                  sw_loadFactor;
    double                  sw_latency;
    timestamp_t             sw_AvaliableTime;
};

#endif // ISPD_EXA_ENGINE_SWITCH_HPP
