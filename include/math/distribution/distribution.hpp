#ifndef ENGINE_DISTRIBUTION_HPP
#define ENGINE_DISTRIBUTION_HPP

#include <core/core.hpp>
#include <random>
#include <type_traits>

/**
 * @brief Two Stage Uniform Distribution class.
 *
 * @tparam RealType a real number type
 */
template <typename RealType>
struct TwoStageUniformDistribution
{
    static_assert(std::is_floating_point<RealType>::value,
                  "TwoStageUniform type must be a floating-point type");
public:
    using value_type = RealType;

    /**
     * @brief Returns a random generated number under the Two Stage Uniform
     *        Distribution with the specified parameters.
     *
     * @param low the lower value
     * @param med the medium value
     * @param hi the higher value
     * @param prob the probability
     * @param engine the random engine
     * @param uniform a uniform real distribution
     *
     * @return a random generated number under the Two Stage Uniform Distribution
     */
    ENGINE_INLINE value_type
    operator()(const double                              low,
               const double                              med,
               const double                              hi,
               const double                              prob,
               std::default_random_engine               &engine,
               std::uniform_real_distribution<RealType> &uniform) const
    {
        double a, b, u;

        u = uniform(engine);

        if (u <= prob) {
            a = low;
            b = med;
        }
        else {
            a = med;
            b = hi;
        }

        return uniform(engine) * (b - a) + a;
    }
};

#endif // ENGINE_DISTRIBUTION_HPP
