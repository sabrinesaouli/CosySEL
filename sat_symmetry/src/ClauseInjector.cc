// Copyright 2017 Hakan Metin - LIP6

#include "cosy/ClauseInjector.h"

namespace cosy {

ClauseInjector::ClauseInjector() {
    _injectors.resize(NR_TYPES);
}

ClauseInjector::~ClauseInjector() {
}


void ClauseInjector::addClause(Type type, BooleanVariable cause,
                               std::vector<Literal>&& literals) {
    _injectors[type].addClause(cause, std::move(literals));
}

void ClauseInjector::removeClause(Type type, BooleanVariable cause) {
    _injectors[type].removeClause(cause);
}

bool ClauseInjector::hasClause(Type type, BooleanVariable cause) const {
    return _injectors[type].hasClause(cause);
}

std::vector<Literal>
ClauseInjector::getClause(Type type, BooleanVariable cause) {
    switch (type) {
    case UNITS:        _stats.units.increment();           break;
    case ESBP:         _stats.esbp.increment();            break;
    case ESBP_FORCING: _stats.esbp_forcing.increment();    break;
    default: CHECK_NOTNULL(nullptr);
    }
    return _injectors[type].getClause(cause);
}

void ClauseInjector::removeClause(BooleanVariable cause) {
    for (Injector& injector : _injectors)
        injector.removeClause(cause);
}

}  // namespace cosy

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
