#ifndef ENGINE_WORKLOAD_HPP
#define ENGINE_WORKLOAD_HPP

#include <chrono>
#include <customer/customer.hpp>
#include <engine.hpp>
#include <math/distribution/distribution.hpp>
#include <random>

/**
 * @brief Szudzik's hash function.
 *
 * @param a the first unsigned 32-bit integer
 * @param b the first unsigned 32-bit integer
 *
 * @return an unsigned 64-bit integer represented the image of the mapping
 *         applied to (a, b)
 */
ENGINE_INLINE static uint64_t szudzik(const uint32_t a, const uint32_t b)
{
    const auto a64 = static_cast<uint64_t>(a);
    const auto b64 = static_cast<uint64_t>(b);
    return a64 >= b64 ? a64 * a64 + a64 + b64 : a64 + b64 * b64;
}

struct Workload
{
public:
    /**
     * @brief Returns a task with its parameters (as processing size and
     *        communication size) loaded by an underlying workload generator
     * policy as well as the arrival time which represents the time the task
     * will arrive at the master.
     *
     *        Further, the task is stamped with its unique identifier
     *        using the `Szudzik's function` taking as input the pair
     *        of values (m_MasterId, m_Count) and producing a unique
     *        task identifier.
     *
     * @param arrivalTime the task arrival time at the master
     *
     * @return the workload generated task
     */
    virtual Task operator()(timestamp_t &arrivalTime) = 0;

protected:
    /**
     * @brief Constructor which specifies the master's identifier to which
     *        the workload will be generated.
     *
     * @param masterId the master's identifier
     */
    explicit Workload(const sid_t masterId)
        : m_MasterId(masterId), m_Count(uint32_t(0))
    {}

    /**
     * @brief It stores the master's identifier which will schedule
     *        all the generated workload.
     */
    sid_t m_MasterId;

    /**
     * @brief This represents an `internal` variable that is used to
     *        store the amount of tasks that have been generated.
     *
     *        When each task is generated, the task identifier is set
     *        together with the processing size and communication size
     *        specifications.
     *
     *        Therefore, to produce in a fast way an uniquely task identifier
     *        among all possible identifiers is used the `Szudzik's function`
     *        taking as input the pair of values (m_MasterId, m_Count) and
     *        producing an unique task identifier.
     */
    uint32_t m_Count;
};

struct NodeWorkload : public Workload
{
public:
    /**
     * @brief Constructor which specifies the master's identifier to which this
     *        workload will be generated, the minimum and maximum processing
     * size in megaflops as well as the minimum and maximum communication size
     *        in megabits of the generated tasks.
     *
     * @param masterId the master's identifier
     * @param minProcSize the minimum processing size in megaflops
     * @param maxProcSize the maximum processing size in megaflops
     * @param minCommSize the minimum communication size in megabits
     * @param maxCommSize the maximum communication size in megabits
     */
    explicit NodeWorkload(const sid_t  masterId,
                          const double minProcSize,
                          const double maxProcSize,
                          const double minCommSize,
                          const double maxCommSize)
        : Workload(masterId), m_MinProcSize(minProcSize),
          m_MaxProcSize(maxProcSize), m_MinCommSize(minCommSize),
          m_MaxCommSize(maxCommSize)
    {}

    Task operator()(timestamp_t &arrivalTime) override
    {
        // Generates the unique task identifier.
        const uint64_t tid = szudzik(m_Count, m_MasterId);

        // Generates the arrival time for this task.
        arrivalTime = m_ExponentialDist(m_Engine);

        // Marks that a task has been created.
        m_Count++;

        const double procSizeMed = (m_MinProcSize + m_MaxProcSize) * 0.5;
        const double commSizeMed = (m_MinCommSize + m_MaxCommSize) * 0.5;

        return Task(tid,
                    // Generate the processing size using a Two Stage Uniform
                    // distribution
                    m_Tsu(m_MinProcSize,
                          procSizeMed,
                          m_MaxProcSize,
                          1,
                          m_Engine,
                          m_UniformDist),
                    // Generate the communication size using a Two Stage Uniform
                    // distribution
                    m_Tsu(m_MinCommSize,
                          commSizeMed,
                          m_MaxCommSize,
                          1,
                          m_Engine,
                          m_UniformDist));
    }

private:
    double m_MinProcSize;
    double m_MaxProcSize;
    double m_MinCommSize;
    double m_MaxCommSize;

    TwoStageUniformDistribution<double> m_Tsu{};

    std::default_random_engine             m_Engine{static_cast<uint_fast32_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count())};
    std::uniform_real_distribution<double> m_UniformDist;
    std::exponential_distribution<timestamp_t> m_ExponentialDist{1.0 / 5.0};
};

#endif // ENGINE_WORKLOAD_HPP
