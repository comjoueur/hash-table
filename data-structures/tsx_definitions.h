#ifndef TSXDEFINITIONS_H
#define TSXDEFINITIONS_H

#include "data-structures/seqcircular.h"

#include "data-structures/simpleelement.h"
#include "data-structures/markableelement.h"
#include "data-structures/tsxcircular.h"
#include "data-structures/strategy/wstrat_user.h"
#include "data-structures/strategy/wstrat_pool.h"
#include "data-structures/strategy/estrat_async.h"
#include "data-structures/strategy/estrat_sync.h"
#include "data-structures/strategy/estrat_sync_alt.h"
#include "data-structures/growtable.h"

namespace growt {

template<class HashFct = std::hash<typename SimpleElement::Key>, class Allocator = std::allocator<char> >
using folklore    = TSXCircular<SimpleElement, HashFct, Allocator>;


template<class E, class HashFct = std::hash<E>, class Allocator = std::allocator<E> >
using NoGrow      = TSXCircular<E, HashFct, Allocator>;

template<class HashFct    = std::hash<typename MarkableElement::Key>,
         class Allocator  = std::allocator<char> >
using uaxGrow  = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratUser, EStratAsync>;

template<class HashFct    = std::hash<typename SimpleElement::Key>,
         class Allocator  = std::allocator<char> >
using usxGrow  = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratUser, EStratSync>;

template<class HashFct    = std::hash<typename SimpleElement::Key>,
         class Allocator  = std::allocator<char> >
using usnxGrow = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratUser, EStratSyncNUMA>;


template<class HashFct    = std::hash<typename MarkableElement::Key>,
         class Allocator  = std::allocator<char> >
using paxGrow  = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratPool, EStratAsync>;

template<class HashFct    = std::hash<typename SimpleElement::Key>,
         class Allocator  = std::allocator<char> >
using psxGrow  = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratPool, EStratSync>;

template<class HashFct   = std::hash<typename SimpleElement::Key>,
         class Allocator = std::allocator<char> >
using psnxGrow = GrowTable<NoGrow<MarkableElement, HashFct, Allocator>, WStratPool, EStratSyncNUMA>;

}

#endif // DEFINITIONS_H
