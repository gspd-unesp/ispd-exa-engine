#ifndef ENGINE_CUSTOMER_HPP
#define ENGINE_CUSTOMER_HPP

#include <core/core.hpp>
#include <cstdint>

/**
 * @brief A customer is a unit of something that can
 *        be processed by some resource.
 */
class Customer
{
};

/**
 * @brief A task represents the smallest unit of work
 *        for a processing resource
 */
class Task : public Customer
{
public:
    /**
     * @brief Constructor which specifies the task identifier, the processing size
     *        in megaflops and the communication size in megabits of this task.
     *
     * @details
     *        It is the caller's responsibility to ensure
     *        that this task identifier is unique.
     *
     *        Further, it is recommended the use of an
     *        automatic task generator that can be accomplished
     *        using `workload generators`, since these generate
     *        uniquely task identifiers transparently.
     *
     * @param tid the task identifier
     * @param processingSize the processing size
     * @param communicationSize the communication size
     */
    explicit Task(const uint64_t tid, const double processingSize, const double communicationSize) noexcept
        : m_Tid(tid), m_ProcSize(processingSize), m_CommSize(communicationSize)
    {}

    /**
     * @brief It returns the processing size in megaflops.
     *
     * @return the processing size in mega-flops
     */
    ENGINE_INLINE double getProcessingSize() const
    {
        return m_ProcSize;
    }

    /**
     * @brief It returns the communication size in megabits.
     *
     * @return the communication size in megabits
     */
    ENGINE_INLINE double getCommunicationSize() const
    {
        return m_CommSize;
    }

    /**
     * @brief Returns the task unique identifier.
     *
     * @return the task unique identifier
     */
    ENGINE_INLINE uint64_t getTid() const
    {
        return m_Tid;
    }

private:
    /**
     * @brief It represents the task id.
     */
    uint64_t m_Tid;

    /**
     * @brief It represents the processing size of this task in megaflops.
     */
    double m_ProcSize;

    /**
     * @brief It represents the communication size of this task in megabits.
     */
    double m_CommSize;
};

#endif // ENGINE_CUSTOMER_HPP
