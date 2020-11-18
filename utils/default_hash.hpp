#pragma once

#if (! (defined(CRC)     || \
        defined(MURMUR2) || \
        defined(MURMUR3) || \
        defined(XXHASH)) )
#define MURMUR2
#endif // NO HASH DEFINED

#ifdef CRC
#include "hash/crc_hash.h"
#define HASHFCT utils_tm::hash_tm::crc_hash
namespace utils_tm {
namespace hash_tm {
    using default_hash = crc_hash;
}
}
#endif //CRC


#ifdef MURMUR2
#include "hash/murmur2_hash.h"
#define HASHFCT utils_tm::hash_tm::murmur2_hash
namespace utils_tm {
namespace hash_tm {
    using default_hash = murmur2_hash;
}
}
#endif // MURMUR2


#ifdef MURMUR3
#include "hash/murmur3_hash.h"
#define HASHFCT utils_tm::hash_tm::murmur3_hash
namespace utils_tm {
namespace hash_tm {
    using default_hash = murmur3_hash;
}
}
#endif // MURMUR3



#ifdef XXHASH
#include "hash/xx_hash.h"
#define HASHFCT utils_tm::hash_tm::xx_hash
namespace utils_tm {
namespace hash_tm {
    using default_hash = xx_hash;
}
}
#endif // XXHASH
