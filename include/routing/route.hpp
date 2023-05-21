#ifndef ENGINE_ROUTING_ROUTE_HPP
#define ENGINE_ROUTING_ROUTE_HPP

#include <core/core.hpp>
#include <cstdint>

/**
 * @brief A route descriptor.
 *
 * @details
 *        The route descriptor is a piece of information of the events
 *        that is used to route packets through a path between the source
 *        and destination services.
 *
 *        The route descriptor stores the following informations.
 *
 *          `src`: This information is the source service's identifier of
 *                 the route. It is used in conjunction with the destination
 *                 service's identifier for index a routing table that contains
 *                 the path between these two services.
 *
 *          `dest`: This information is the destination service's identifier of
 *                  the route. It is used in conjunction with the source
 *                  service's  identifier for index a routing table that
 *                  contains the path between these two services.
 *
 *          `offset: Once we have the route between these two services, this
 *                   information is used to identify which service in the route
 *                   is the next to receive the packet.
 */
class RouteDescriptor
{
public:
    /**
     * @brief Default ctor.
     */
    explicit RouteDescriptor()
    {
        m_Src    = static_cast<uint64_t>(-1);
        m_Dest   = static_cast<uint64_t>(-1);
        m_Offset = 0;
    }

    /**
     * @brief Construct a route descriptor specifying the source service,
     *        the destination service and the offset for indexing the next
     *        service in the route between the source and destination services.
     *
     * @param src the source service identifier
     * @param dest the destination service identifier
     * @param offset an offset used for indexing the next service's identifier
     *               in the route between the source and destination services
     */
    explicit RouteDescriptor(const uint64_t    src,
                             const uint64_t    dest,
                             const std::size_t offset)
    {}

    /**
     * @brief Returns the route source service's identifier.
     *
     * @return the route source service's identifier
     */
    ENGINE_INLINE uint64_t getSource() const
    {
        return m_Src;
    }

    /**
     * @brief Returns the route destination service's identifier.
     *
     * @return the route destination service's identifier
     */
    ENGINE_INLINE uint64_t getDestination() const
    {
        return m_Dest;
    }

    /**
     * @brief Returns the route offset that is used for indexing
     *        the next service's identifier in the route.
     *
     * @return the route offset used that is for indexing the
     *         next service's identifier in the route
     */
    ENGINE_INLINE std::size_t getOffset() const
    {
        return m_Offset;
    }

private:
    /**
     * @brief The route source service's identifier.
     */
    uint64_t m_Src;

    /**
     * @brief The route destination service's identifier.
     */
    uint64_t m_Dest;

    /**
     * @brief The route offset.
     *
     * @details
     *        The route offset is used to index the next service's
     *        identifier in a route.
     *
     *        For instance, suppose we have the route containing the
     *        following services identifiers.
     *
     *          (l1, l2, l3, l4, l5)
     *
     *        Therefore, if the route offset is equal to 2, then the
     *        next element to forward the packet is the service which
     *        has the identifier `l3`.
     *
     *        Note that neither `l1` nor `l5` is the source or the
     *        destination service, respectively. For more information
     *        in how the route is represented, look at the documentation
     *        provided by the `Route` and `RoutingTable` classes.
     */
    std::size_t m_Offset;
};

#endif // ENGINE_ROUTING_ROUTE_HPP
