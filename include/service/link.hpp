#ifndef ENGINE_LINK_HPP
#define ENGINE_LINK_HPP

#include <service/service.hpp>

struct LinkMetrics
{
    double m_CommMBits;
    double m_CommTime;
    double m_CommTasks;
};

class Link : public Service
{
public:
    /**
     * @brief Constructor which specifies the link's identifier, outgoing connection, ingoing connection,
     *        the bandwidth in megabits, the load factor and the amount of latency.
     *
     * @param id the link's identifier
     * @param from the outgoing connection
     * @param to the ingoing connection
     * @param bandwidth the bandwidth in megabits
     * @param loadFactor the load factor (a value in the interval [0, 1])
     * @param latency the latency in seconds
     */
    explicit Link(const sid_t id, const sid_t from, const sid_t to, const double bandwidth, const double loadFactor,
                  const double latency)
        : Service(id), m_From(from), m_To(to), m_Bandwidth(bandwidth), m_LoadFactor(loadFactor), m_Latency(latency),
          m_AvailableTime(0.0)
    {}

    /**
     * @brief It calculates the time taken in seconds by the link to communicate
     *        a customer with the specified communication size in megabits.
     *
     * @details
     *        It is the caller's responsibility to ensure that the communication
     *        size is a non-negative real number.
     *
     * @param commSize the communication size (in megabits)
     *
     * @return the time taken in seconds by the link to process a customer with the
     *         specified communication size in megabits
     */
    ENGINE_INLINE double timeToCommunicate(const double commSize) const
    {
        return m_Latency + commSize / ((1.0 - m_LoadFactor) * m_Bandwidth);
    }

    void onTaskArrival(timestamp_t, const Event *event) override;

    /**
     * @brief Returns a const (read-only) reference to the machine metrics.
     *
     * @return a const (read-only) reference to the machine metrics
     */
    const LinkMetrics &getMetrics() const
    {
        return m_Metrics;
    }

    ENGINE_TEMPORARY timestamp_t getLocalVirtualTime() const
    {
        return m_Lvt;
    }

private:
    LinkMetrics                  m_Metrics{};
    sid_t                        m_From;
    sid_t                        m_To;
    double                       m_Bandwidth;
    double                       m_Latency;
    double                       m_LoadFactor;
    timestamp_t                  m_AvailableTime;
    ENGINE_TEMPORARY timestamp_t m_Lvt{};
};

#endif // ENGINE_LINK_HPP
