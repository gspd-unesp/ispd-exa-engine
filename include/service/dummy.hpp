#ifndef ENGINE_DUMMY_HPP
#define ENGINE_DUMMY_HPP

#include <service/service.hpp>

class Dummy : public Service
{
public:
    explicit Dummy(const sid_t id) : Service(id)
    {}

    void onTaskArrival(timestamp_t time, const Event *event) override
    {
        // Does nothing.
    }
};

#endif // ENGINE_DUMMY_HPP
