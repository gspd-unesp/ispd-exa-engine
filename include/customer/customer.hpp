#ifndef ENGINE_CUSTOMER_HPP
#define ENGINE_CUSTOMER_HPP

#include <core/core.hpp>

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
     * @brief Constructor which specifies the processing size in megaflops
     *        and the communication size in megabits of this task.
     *
     * @param processingSize the processing size in megaflops
     * @param communicationSize the communication size in megabits
     */
    explicit Task(const double processingSize, const double communicationSize) noexcept
        : m_ProcSize(processingSize), m_CommSize(communicationSize)
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

private:
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
