#ifndef ENGINE_WORKLOAD_DISTRIBUTION_HPP
#define ENGINE_WORKLOAD_DISTRIBUTION_HPP

#include <ROOT-Sim.h>

/// \brief The `WorkloadDistribution` class is an abstract base class that
///        defines the interface for different types of workload distribution
///        models.
///
/// \note Subclasses must implement the `setArrivalTime` method to
///       modify the arrival time based on the specific distribution.
class WorkloadDistribution
{
public:
    /// \brief Modifies the arrival time based on the specific distribution
    ///        model.
    ///
    /// This method is called to update the arrival time of a task based on the
    /// characteristics of the workload distribution. Subclasses are responsible
    /// for implementing this method according to their specific distribution
    /// logic.
    ///
    /// \param timestamp The current arrival time, which will be updated
    ///                  according to the distribution.
    virtual void setArrivalTime(double &timestamp) = 0;
};

/// \brief The `FixedWorkloadDistribution` class is a concrete subclass of
///        `WorkloadDistribution` that represents a fixed interarrival time
///        distribution.
///
/// This distribution model adds a constant offset to the arrival time of tasks.
/// The offset value is specified during the construction of the
/// `FixedWorkloadDistribution` object.
///
/// \note The `setArrivalTime` method overrides the base class method and
///       modifies the `timestamp` parameter by adding the offset value. This
///       ensures that the arrival time is adjusted based on the fixed
///       interarrival time distribution.
///
/// Example Usage:
/// ```
/// double arrivalTime = 10.0;
/// double offset = 5.0;
///
/// FixedWorkloadDistribution distribution(offset);
/// distribution.setArrivalTime(arrivalTime);
///
/// ```
///
/// In the previous case, when `setArrivalTime` is called with an `arrivalTime`
/// of 10.0 and the `FixedWorkloadDistribution` object has an offset of 5.0, the
/// `arrivalTime` will be updated to 15.0.
///
/// Further, if `setArrivalTime` is called again with the updated `arrivalTime`
/// value of 15.0 and the same `FixedWorkloadDistribution` object, the
/// `arrivalTime` will be further updated to 20.0, since the offset is applied
/// cumulatively each time the method is called.
class FixedWorkloadDistribution : public WorkloadDistribution
{
public:
    /// \brief Constructs a `FixedWorkloadDistribution` object with the
    ///        specified offset.
    ///
    /// \param offset The constant offset to be added to the arrival time of
    ///               tasks.
    explicit FixedWorkloadDistribution(const double offset) : m_Offset(offset)
    {}

    /// \brief Modifies the arrival time by adding the fixed offset.
    ///
    /// This method overrides the base class `setArrivalTime` method and
    /// modifies the `timestamp` parameter by adding the constant offset value.
    /// The result is an updated arrival time based on the fixed interarrival
    /// time distribution.
    ///
    /// \param timestamp The current arrival time, which will be updated by
    /// adding the offset.
    void setArrivalTime(double &timestamp) override
    {
        timestamp += m_Offset;
    }

private:
    /// \brief The constant offset for the fixed interarrival time distribution.
    double m_Offset;
};

/// \brief A concrete implementation of the WorkloadDistribution class that
///        models the interarrival task time distribution using a Poisson
///        distribution.
///
/// The PoissonWorkloadDistribution class generates interarrival times according
/// to a Poisson distribution with a specified lambda parameter. The
/// setArrivalTime method updates the given timestamp by adding a randomly
/// generated value from the Poisson distribution.
///
/// \note The lambda parameter controls the average rate at which events occur.
class PoissonWorkloadDistribution : public WorkloadDistribution
{
public:
    /// \brief Constructs a PoissonWorkloadDistribution object with the
    ///        specified lambda parameter.
    ///
    /// \param lambda The lambda parameter of the Poisson distribution.
    explicit PoissonWorkloadDistribution(const double lambda) : m_Lambda(lambda)
    {}

    /// \brief Updates the given timestamp by adding a randomly generated value
    ///        from the Poisson distribution.
    ///
    /// \param timestamp The timestamp to be updated.
    void setArrivalTime(double &timestamp) override
    {
        timestamp += Expent(m_Lambda);
    }

private:
    /// \brief The lambda parameter of the Poisson distribution.
    ///
    /// \note The lambda parameter in a Poisson distribution represents the
    ///       average rate at which events occur in a fixed interval of time or
    ///       space. It determines the shape and intensity of the distribution.
    ///       A higher lambda value indicates a higher average rate of events,
    ///       while a lower lambda value indicates a lower average rate of
    ///       events.
    double m_Lambda;
};

#endif // ENGINE_WORKLOAD_DISTRIBUTION_HPP
