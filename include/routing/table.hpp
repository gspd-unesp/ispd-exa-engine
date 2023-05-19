#ifndef ENGINE_ROUTING_TABLE_HPP
#define ENGINE_ROUTING_TABLE_HPP

#include <core/core.hpp>
#include <cstdint>
#include <math/utility.hpp>
#include <string>
#include <unordered_map>

class Route
{
public:
    explicit Route(const std::size_t length, uint32_t *path) noexcept
        : m_Length(length), m_Path(path)
    {}

    uint32_t operator[](std::size_t n) const
    {
        if (UNLIKELY(n < 0 || n >= m_Length))
            die("Invalid path index accessed");
        return m_Path[n];
    }

private:
    std::size_t m_Length;
    uint32_t   *m_Path;
};

class RoutingTable
{
public:
    ENGINE_INLINE
    void emplaceRoute(const uint32_t src, const uint32_t dest, Route &&route)
    {
        m_RoutingTable.emplace(szudzik(src, dest), std::move(route));
    }

    ENGINE_INLINE
    const Route &getRoute(const uint32_t src, const uint32_t dest)
    {
        return m_RoutingTable[szudzik(src, dest)];
    }

private:
    std::unordered_map<uint64_t, Route> m_RoutingTable;
};

class RoutingTableReader
{
public:
    RoutingTable *read(std::string filepath)
    {
        return nullptr;
    }
};
#endif // ENGINE_ROUTING_TABLE_HPP
