#ifndef RETURNELEMENT_H
#define RETURNELEMENT_H

#include <stdlib.h>
#include <tuple>

namespace growt {

enum class ReturnCode
{
    // bit pattern:  1 = success ;   2 = not found key ;   4 = found key
    //               8 = insert  ;  16 = update        ;  32 = delete
    //              64 = full    ; 128 = invalid cell  ; 256 = TSX

    ERROR                      = 0,

    SUCCESS_IN                 = 9,   // success+insert
    SUCCESS_UP                 = 17,  // success+update
    SUCCESS_DEL                = 33,  // success+delete

    UNSUCCESS_NOT_FOUND        = 2,
    UNSUCCESS_ALREADY_USED     = 4,
    UNSUCCESS_FULL             = 64,
    UNSUCCESS_INVALID          = 128,

    TSX_SUCCESS_IN             = 265, // success+insert+TSX
    TSX_SUCCESS_UP             = 273, // success+update+TSX
    TSX_SUCCESS_DEL            = 289, // success+delete+TSX

    TSX_UNSUCCESS_NOT_FOUND    = 258, // not found   +TSX
    TSX_UNSUCCESS_ALREADY_USED = 260, // already used+TSX
    TSX_UNSUCCESS_FULL         = 320, // full        +TSX
    TSX_UNSUCCESS_INVALID      = 384, // invalid     +TSX

    TSX_ABORT                  = 256  // TSX+ERROR
};

inline bool successful(ReturnCode ec)
{
    return (static_cast<uint>(ec) & 1u);
}

}

#endif // RETURNELEMENT_H
