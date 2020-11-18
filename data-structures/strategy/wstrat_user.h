#ifndef WSTRAT_USER_H
#define WSTRAT_USER_H

#include <cstddef>
#include <atomic>

namespace growt {

template <class Parent>
class WStratUser
{
public:

    // The enslavement strategy, does not actually need any global data
    class global_data_t
    {
    public:
        global_data_t() {}
        global_data_t(const global_data_t& source) = delete;
        global_data_t& operator=(const global_data_t& source) = delete;
        ~global_data_t() = default;
    };


    // No initialization or deinitialization needed.
    class local_data_t
    {
    public:
        local_data_t(Parent &parent) : _parent(parent) { }
        local_data_t(const local_data_t& source) = delete;
        local_data_t& operator=(const local_data_t& source) = delete;
        local_data_t(local_data_t&& source) = default;
        local_data_t& operator=(local_data_t&& source) = default;
        ~local_data_t() = default;

        Parent &_parent;

        template<class EStrat>
        inline void init(EStrat&) { }
        inline void deinit() {}

        template<class ESLocal>
        inline void execute_migration(ESLocal &estrat, size_t)
        {
            estrat.migrate();
        }
    };
};

}

#endif // WSTRAT_USER_H
