#ifndef ENGINE_ROUTING_ROUTE_HPP
#define ENGINE_ROUTING_ROUTE_HPP

#include <engine.hpp>

class RouteDescriptor
{
private:
    /**
     * @brief A route descriptor contains the essential information
     *        to describe the fastest path from a source vertex to
     *        a destination vertex.
     *
     *        Further, this descriptor it is composed by three informations
     *        that are the source service's identifier, the destination
     *        service's identifier and the route's offset.
     *
     *                                                   ROUTE OFFSET
     *           SOURCE VERTEX ID        DEST VERTEX ID         
     *        63                  34 33                    4 3  0
     *        000000000...000000000  0000000000...0000000000 0000
     */
    uint64_t m_Desc;
};

#endif // ENGINE_ROUTING_ROUTE_HPP
