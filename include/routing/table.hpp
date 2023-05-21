#ifndef ENGINE_ROUTING_TABLE_HPP
#define ENGINE_ROUTING_TABLE_HPP

#include <core/core.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <math/utility.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief A route is used to indicate the services in a path that must be
 *        passed through for arrive at a specified destination service
 *        starting from a specified source service.
 *
 * @details
 *        Although, in a route from a source service to a destination services
 *        may be necessary to pass through many masters, machines and links.
 *        It is only necessary to store the links' identifiers inside the route,
 *        since the link is a graph edge and, therefore, it is only connected to
 *        two services and, then the link already know which is the next service
 *        in the route.
 *
 *        Furthermore, it is not stored inside the route the source service and
 *        the destination service by the same reason give above.
 *
 *        Example:
 *
 *          M1 -> l1 -> m1 -> l2 -> m2 -> l3 -> M2 -> l4 -> m3 -> l5 -> m4
 *
 *          A route starting from M1 and arriving at m4 is:
 *          (l1, l2, l3, l4, l5)
 *
 */
class Route
{
public:
    /**
     * @brief Constructor which specifies the route's length and
     *        the route's path.
     *
     * @param length the route's length
     * @param path the route's path, that is composed by the
     *             services' identifiers that are inner to this path
     *             (i.e., it is neither the source nor the vertex service)
     */
    explicit Route(std::size_t length, std::uint32_t *path) noexcept
        : m_Length(length), m_Path(path)
    {}

    /**
     * @brief Returns the path's length.
     *
     * @return the path's length
     */
    ENGINE_INLINE std::size_t getLength() const
    {
        return m_Length;
    }

    /**
     * @brief Returns the service's identifier of the n-th path element.
     *
     * @return the service's identifier of the n-th path element.
     */
    ENGINE_INLINE std::uint32_t operator[](std::size_t n) const
    {
        return m_Path[n];
    }

private:
    std::size_t    m_Length;
    std::uint32_t *m_Path;
};

/**
 * @brief A routing table is used to store the shortest route between
 *        two services and to retrieve it fast.
 *
 * @details
 *        Mathematically speaking, our route definition is a triple (u, v, p)
 *        in which `u` is the source service, `v` is the destination service and
 * `p` is the path between these two services.
 *
 *        Our routing table is basically a hash table that maps the (u, v) pair
 *        to the path p, that is, it maps (u, v) -> p. Therefore, the route
 * itself that is represented by `p` do not need to contains the `u` and `v`
 * services.
 *
 *        Hence, it is by these reasons that the path do not contains the source
 * and destination services.
 *
 *        For instance, suppose the following topology.
 *
 *          M1 -> L1 -> m1 -> L2 -> m2 -> L3 -> m3 -> L4 -> m4
 *
 *        That represents a linear topology. Therefore, the shortest route
 *        between M1 and m3 in the mathematical definition given above is
 *
 *          (M1, m3, [L1, L2, L3])
 *
 *        Note that is only necessary to store the link service type in the
 * route.
 */
class RoutingTable
{
public:
    /**
     * @brief Add a route in the table.
     *
     *        If a route has already been defined to the specified source and
     *        destination services, then the current call to adding a route
     *        will replace the old route by the current specified route.
     */
    ENGINE_INLINE
    void addRoute(const uint32_t src, const uint32_t dest, const Route *route)
    {
        m_RoutingTable.insert(std::make_pair(szudzik(src, dest), route));
    }

    /**
     * @brief Returns the registered route between the specified source and
     *        destination services.
     *
     *        However, if no route has previously been defined to these
     *        services, then `nullptr` is returned.
     *
     * @return the registered route between the specified source and destination
     *         services; however, if no route has previously been defined to
     *         these services, then `nullptr` is returned.
     */
    ENGINE_INLINE
    const Route *getRoute(const uint32_t src, const uint32_t dest) const
    {
        return m_RoutingTable.at(szudzik(src, dest));
    }

    /**
     * @brief Returns the amount of routes that have been registered.
     *
     * @return the amount of routes that have been registered
     */
    ENGINE_INLINE
    std::size_t getRoutesSize() const
    {
        return m_RoutingTable.size();
    }

private:
    /**
     * @brief An unordered map representing the routing table that stores a
     *        pair containing a 64-bit unsigned integer that represents the
     *        resulting value from the Szudzik's pairing function applied to
     *        the source and destination service's identifier, and containing
     *        the route between these source and destination services.
     */
    std::unordered_map<uint64_t, const Route *> m_RoutingTable;
};

class RoutingTableReader
{
public:
    /**
     * @brief Reads a file in the `.route` format containing the routes between
     *        two services and construct the routing table with that read
     * information.
     */
    RoutingTable *read(const std::string &filepath)
    {
        std::ifstream file(filepath);

        // It checks if the file could not be opened for some reason. If so,
        // then the program is immediately aborted.
        if (!file.is_open())
            die("Routing file '%s' could not be opened", filepath.c_str());

        RoutingTable *rt = new RoutingTable();

        // Read the file. Each line in the file contains a route indicating
        // the source service, the destination service and the services'
        // identifiers that composes the inner route's element.
        for (std::string routeLine; std::getline(file, routeLine);) {
            uint32_t src;
            uint32_t dest;
            Route   *route = readRoute(routeLine, src, dest);
            rt->addRoute(src, dest, route);
        }

        file.close();
        return rt;
    }

private:
    /**
     * @brief A parsing stage is an enumeration that is used
     *        classify the current parsing stage of the routing
     *        table reader.
     */
    enum ParsingStage
    {
        /**
         * @brief This parsing stage indicates that the source service
         *        identifier is being parsed by the reader.
         */
        SOURCE_VERTEX,

        /**
         * @brief This parsing stage indicates that the destination service
         *        identifier is being parsed by the reader.
         */
        DESTINATION_VERTEX,

        /**
         * @brief This parsing stage indicates that the route service
         *        identifier is being parsed by the reader.
         */
        INNER_VERTEX
    };

    Route *readRoute(const std::string &routeLine,
                     uint32_t          &src,
                     uint32_t          &dest)
    {
        const std::size_t routeLineLength = routeLine.length();
        std::size_t       whitespaceCount = 0;

        std::size_t    pathLength = 0;
        std::size_t    pathIndex  = 0;
        std::uint32_t *path       = nullptr;

        // It counts the amount of whitespaces the route line contains.
        // With that information in hands, it is possible to conclude the
        // path length and allocate the exactly amount of path elements.
        for (std::size_t i = 0; i < routeLineLength; i++)
            if (routeLine[i] == ' ')
                whitespaceCount++;

        // It sets the path length and allocate the path elements.
        pathLength = whitespaceCount - 1;
        path       = new std::uint32_t[pathLength];

        std::size_t  partStart  = 0;
        std::size_t  partLength = 0;
        ParsingStage stage      = ParsingStage::SOURCE_VERTEX;

        for (std::size_t i = 0; i < routeLineLength; i++) {
            while (routeLine[i] != ' ' && i < routeLineLength) {
                partLength++;
                i++;
            }

            // It obtains a route part. That is represented by a sequence of
            // letters followed by a space, new line or end of file.
            const std::string routePart =
                routeLine.substr(partStart, partLength);

            switch (stage) {
            case ParsingStage::SOURCE_VERTEX:
                src   = std::stoul(routePart);
                stage = ParsingStage::DESTINATION_VERTEX;
                break;
            case ParsingStage::DESTINATION_VERTEX:
                dest  = std::stoul(routePart);
                stage = ParsingStage::INNER_VERTEX;
                break;
            case ParsingStage::INNER_VERTEX:
                path[pathIndex++] = std::stoul(routePart);
                break;
            default:
                std::cerr << "Unknown parsing stage." << std::endl;
                exit(EXIT_FAILURE);
            }

            partStart  = i + 1;
            partLength = 0;
        }

        return new Route(pathLength, path);
    }
};

#endif // ENGINE_ROUTING_TABLE_HPP
