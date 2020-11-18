
#pragma once

#include <tuple>

template <class K, class D>
class StupidIterator
{
private:
    std::pair<K,D> pair;

public:
    using key_type = K;
    using mapped_type = D;
    using value_type  = std::pair<K,D>;
    using reference   = value_type&;

    StupidIterator(const key_type& k = key_type(),
                   const mapped_type& d = mapped_type())
        : pair(k,d) { }

    const std::pair<K,D>& operator*()
    {
        return pair;
    }

    inline bool operator==(const StupidIterator& r) const
    { return pair.first == r.pair.first; }
    inline bool operator!=(const StupidIterator& r) const
    { return pair.first != r.pair.first; }
};
