#ifndef ESTRAT_ASYNC_H
#define ESTRAT_ASYNC_H

#include <atomic>
#include <mutex>
#include <memory>

#include "data-structures/markableelement.h"

namespace growt {

template<class Table_t> size_t blockwise_migrate(Table_t source, Table_t target);


template<class Parent>
class EStratAsync
{
public:
    using BaseTable_t   = typename Parent::BaseTable_t;
    using HashPtrRef    = std::shared_ptr<BaseTable_t>&;
    using HashPtr       = std::shared_ptr<BaseTable_t>;

    static_assert(std::is_same<typename BaseTable_t::value_intern, MarkableElement>::value,
                  "Asynchroneous migration can only be chosen with MarkableElement!!!" );

    class local_data_t;

    // STORED AT THE GLOBAL OBJECT
    //  - SHARED POINTERS TO BOTH THE CURRENT AND THE TARGET TABLE
    //  - VERSION COUNTERS
    //  - MUTEX FOR SAVE CHANGING OF SHARED POINTERS
    class global_data_t
    {
    public:
        global_data_t(size_t size_) : _g_epoch_r(0), _g_epoch_w(0), _n_helper(0)
        {
            _g_table_r = _g_table_w = std::make_shared<BaseTable_t>(size_);
        }
        global_data_t(const global_data_t& source) = delete;
        global_data_t& operator=(const global_data_t& source) = delete;
        ~global_data_t() = default;

    private:
        friend local_data_t;

        std::atomic_size_t _g_epoch_r;
        std::atomic_size_t _g_epoch_w;
        HashPtr _g_table_r;
        HashPtr _g_table_w;

        std::mutex _grow_mutex;
        std::atomic_size_t _n_helper;
        // return _g_epoch_r.load(std::memory_order_acquire); }
    };

    // STORED AT EACH HANDLE
    //  - CACHED TABLE (SHARED PTR) AND VERSION NUMBER
    //  - CONNECTIONS TO THE  WORKER STRATEGY AND THE GLOBAL TABLE
    class local_data_t
    {
    private:
        using WorkerStratL  = typename Parent::WorkerStrat_t::local_data_t;
    public:
        local_data_t(Parent& parent, WorkerStratL& wstrat)
            : _parent(parent), _global(parent._global_exclusion),
              _worker_strat(wstrat),
              _epoch(0), _table(nullptr)
        { }

        local_data_t(const local_data_t& source) = delete;
        local_data_t& operator=(const local_data_t& source) = delete;

        local_data_t(local_data_t&& source) = default;
        local_data_t& operator=(local_data_t&& source) = default;
        ~local_data_t() = default;

        inline void init() { load(); }
        inline void deinit() { }

    private:
        Parent&        _parent;
        global_data_t& _global;
        WorkerStratL&  _worker_strat;

        size_t _epoch;
        std::shared_ptr<BaseTable_t> _table;


    public:
        inline HashPtrRef get_table()
        {
            size_t t_epoch = _global._g_epoch_r.load(std::memory_order_acquire);
            if (t_epoch > _epoch)
            {
                load();
            }
            return _table;
        }

        inline void rls_table() {   }

        void grow()
        {
            //std::shared_ptr<BaseTable_t> w_table;
            { // should be atomic (therefore locked)
                std::lock_guard<std::mutex> lock(_global._grow_mutex);
                if (_global._g_table_w->_version == _table->_version)
                {
                    // first one to get here allocates new table
                    auto w_table = std::make_shared<BaseTable_t>(
                       BaseTable_t::resize(_table->_capacity,
                           _parent._elements.load(std::memory_order_acquire),
                           _parent._dummies.load(std::memory_order_acquire)),
                       _table->_version+1);

                    _global._g_table_w = w_table;
                    _global._g_epoch_w.store(w_table->_version,
                                           std::memory_order_release);
                }
            }

            _worker_strat.execute_migration(*this, _epoch);

            end_grow();
        }


        void help_grow()
        {
            _worker_strat.execute_migration(*this, _epoch);
            end_grow();
        }

        inline size_t migrate()
        {
            // enter_migration(): nhelper ++
            _global._n_helper.fetch_add(1, std::memory_order_acq_rel);

            // getCurr() and getNext()
            HashPtr curr, next;
            {
                std::lock_guard<std::mutex> lock(_global._grow_mutex);
                curr = _global._g_table_r;
                next = _global._g_table_w;
            }

            if (curr->_version >= next->_version)
            {
                // late to the party
                // leave_migration(): nhelper --
                _global._n_helper.fetch_sub(1, std::memory_order_release);

                return next->_version;
            }

            //global.g_count.fetch_add(
            blockwise_migrate(curr, next);//,
            //std::memory_order_acq_rel);


            // leave_migration(): nhelper --
            _global._n_helper.fetch_sub(1, std::memory_order_release);

            return next->_version;
        }

    private:
        inline void load()
        {
            {
                std::lock_guard<std::mutex> lock(_global._grow_mutex);
                _epoch = _global._g_epoch_r.load(std::memory_order_acquire);
                _table = _global._g_table_r;
            }
        }

        inline void end_grow()
        {
            //wait for other helpers
            while (_global._n_helper.load(std::memory_order_acquire)) { }

            //CAS table into R-Position
            {
                std::lock_guard<std::mutex> lock(_global._grow_mutex);
                if (_global._g_table_r->_version == _epoch)
                {
                    _global._g_table_r = _global._g_table_w;
                    _global._g_epoch_r.store(_global._g_epoch_w.load(std::memory_order_acquire),
                                           std::memory_order_release);

                    //auto temp = global.g_count.load(std::memory_order_acquire);
                    //parent.elements.store(temp, std::memory_order_release);
                    //parent.dummies .store(   0, std::memory_order_release);
                    //global.g_count .store(   0, std::memory_order_release);


                    auto temp = _parent._dummies.exchange(0, std::memory_order_acq_rel);
                    _parent._elements.fetch_sub(temp, std::memory_order_release);
                }
            }

            load();
        }
    };
};

}

#endif // ESTRAT_ASYNC_H
