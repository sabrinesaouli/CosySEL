// Copyright 2017 Hakan Metin

#ifndef INCLUDE_COSY_AUTOMORPHISMFINDER_H_
#define INCLUDE_COSY_AUTOMORPHISMFINDER_H_

#include <vector>
#include <numeric>
#include <memory>
#include <utility>

#include "cosy/ColoredGraph.h"
#include "cosy/Group.h"
#include "cosy/LiteralGraphNodeAdaptor.h"

#include "cosy/IntegralTypes.h"
#include "cosy/Logging.h"

// #ifdef USE_BLISS
// #include "bliss/graph.hh"
// #endif  // USE_BLISS

// #ifdef USE_SAUCY
// #include "saucy/saucy.h"
// #endif


namespace cosy {

using Adaptor = std::unique_ptr<LiteralGraphNodeAdaptor>;

struct AutomorphismInfo {
    AutomorphismInfo(unsigned int n, LiteralGraphNodeAdaptor* a, Group *g)
        : num_vars(n), adaptor(a), group(g) {}
    unsigned int num_vars;
    LiteralGraphNodeAdaptor *adaptor;
    Group *group;
};


class AutomorphismFinder {
    virtual void findAutomorphisms(unsigned int num_vars,
                                   const Adaptor& adaptor,
                                   Group *group) = 0;
};

}  // namespace cosy

#endif  // INCLUDE_COSY_AUTOMORPHISMFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
