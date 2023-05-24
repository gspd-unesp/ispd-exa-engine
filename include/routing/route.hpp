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
        m_Src                 = static_cast<uint64_t>(-1);
        m_Dest                = static_cast<uint64_t>(-1);
        m_PreviousService     = static_cast<uint64_t>(-1);
        m_Offset              = 0;
        m_ForwardingDirection = true;
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
     * @param forwardingDirection this flag is used to indicate the direction of
     *                            forwarding of the packet.
     */
    explicit RouteDescriptor(const uint64_t    src,
                             const uint64_t    dest,
                             const uint64_t    previousService,
                             const std::size_t offset,
                             const bool        forwardingDirection)
        : m_Src(src), m_Dest(dest), m_PreviousService(previousService),
          m_Offset(offset), m_ForwardingDirection(forwardingDirection)
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

    /**
     * @brief Returns the previous service that has forwarded this route
     *        descriptor.
     *
     * @return the previous service that has forwarded this route descriptor
     */
    ENGINE_INLINE uint64_t getPreviousService() const
    {
        return m_PreviousService;
    }

    /**
     * @brief Returns the direction in which te packet is being forwarded
     *        in a route. If the value is `true`, then the direction of
     *        forwarding is from the master to the slave. Otherwise, if the
     *        value is `false`, then the direction of forwardding is from the
     *        slave to the master.
     *
     * @return the direction in which the packet is being forwarded in a route
     */
    ENGINE_INLINE bool getForwardingDirection() const
    {
        return m_ForwardingDirection;
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
     * @brief The previous service that has forwarded this descriptor.
     *
     * @details
     *        Suppose we have the following topology
     *
     *          M1 -> l1 -> m1 -> l2 -> m2 -> l3 -> m3 -> l4 -> m4
     *
     *        Then, assume we have the following route between the M1
     *        and m4.
     *
     *          (l1, l2, l3, l4)
     *
     *        Therefore, we are in the service `m2`, then it will forward
     *        the packet to the link `l3`, such that the `m_PreviousService`
     *        of the route descriptor when `l3` receives the packet will
     *        be `m2`. With that, the link will know which direction to
     *        forward the packet.
     */
    uint64_t m_PreviousService;

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

    /**
     * @brief The route forward direction.
     *
     * @details
     *       This flag indicates if the packet in the route should be forwarded
     *       in the direction from the master to the slave or vice-versa.
     *       Therefore, this can be indicated by turning on/off this flag.
     *
     *       If this flag is `true`, then the direction of forwarding is from
     *       the mater to the slave. Otherwise, the direction of forwarding is
     *       from the slave to the master.
     */
    bool m_ForwardingDirection;
};

#endif // ENGINE_ROUTING_ROUTE_HPP
