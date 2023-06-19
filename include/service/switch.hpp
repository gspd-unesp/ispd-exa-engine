//
// Created by willao on 19/06/23.
//

#ifndef ISPD_EXA_ENGINE_SWITCH_HPP
#define ISPD_EXA_ENGINE_SWITCH_HPP
#include "link.hpp"
#include "service.hpp"
struct switchMetrics
{
    double m_CommMBits;
    double m_CommTime;
    double m_CommTasks;
   // double incoming_traffic;
};

/**
 * Switch is responsible for fowarding messages to other
 * machines through the links
 */
class Switch : public Service
{
public:/**
 * @brief constructor which especifies the Switch's id, its connected links, bandwidth (megabits)
 *        and the amount of latency
 *
 * @param id switch's id
 * @param connected_links array of links which are connected in the switch
 * @param bandwidth  bandwidth in megabits
 * @param latency      latency
 */
    explicit Switch(const sid_t id, const Link *connected_links, const double bandwidth, const double latency)
        : Service(id), connected_links(connected_links), m_Bandwidth(bandwidth), m_Latency(latency),
          m_AvaliableTime(0.0)
    {}

    void onTaskArrival(timestamp_t, const Event *event) override;

    /**
     * return a read only reference of  switch's metrics
     */
    const switchMetrics &getMetrics const
        {
            return swMetrics;
        };


private:
    Link                    connected_links[NUMBER_PORTS]; //stores the links connected to it's ports
    switchMetrics           swMetrics{};
    timestamp_t             m_AvaliableTime;
    double                  m_Bandwidth;
    double                  m_Latency;
};


#endif // ISPD_EXA_ENGINE_SWITCH_HPP
