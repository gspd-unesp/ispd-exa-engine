#ifndef ENGINE_WORKLOAD_DISTRIBUTION_HPP
#define ENGINE_WORKLOAD_DISTRIBUTION_HPP

#include <ROOT-Sim.h>

class WorkloadDistribution
{
public:
    virtual void setArrivalTime(double &timestamp) = 0;
};

class FixedWorkloadDistribution : public WorkloadDistribution
{
public:
    void setArrivalTime(double &timestamp) override
    {
        timestamp += m_Offset;
    }

private:
    double m_Offset;
};

class PoissonWorkloadDistribution : public WorkloadDistribution
{
public:
    explicit PoissonWorkloadDistribution(const double lambda) : m_Lambda(lambda)
    {}

    void setArrivalTime(double &timestamp) override
    {
        timestamp += Expent(m_Lambda);
    }

private:
    double m_Lambda;
};

#endif // ENGINE_WORKLOAD_DISTRIBUTION_HPP
