// Copyright 2017 Hakan Metin - LIP6

#include "cosy/CosyManager.h"

namespace cosy {

static const bool FLAGS_esbp = true;

CosyManager::CosyManager(const Group& group, const Assignment& assignment) :
    _group(group),
    _assignment(assignment),
    _order(nullptr) {
}

CosyManager::~CosyManager() {
}


void CosyManager::defineOrder(std::unique_ptr<Order>&& order) {
    _order = std::move(order);

    for (const std::unique_ptr<Permutation>& perm : _group.permutations()) {
        std::unique_ptr<CosyStatus> status
            (new CosyStatus(*perm, *_order, _assignment));
        _statuses.emplace_back(status.release());
    }

    for (const Literal& literal : *_order) {
        const BooleanVariable variable = literal.variable();
        for (const unsigned int& index : _group.watch(variable))
            _statuses[index]->addLookupLiteral(literal);
    }
}

void CosyManager::generateUnits(ClauseInjector *injector) {
    for (const std::unique_ptr<CosyStatus>& status : _statuses)
        status->generateUnitClauseOnInverting(injector);
}

void CosyManager::updateNotify(const Literal& literal,
                               ClauseInjector *injector) {
    IF_STATS_ENABLED(ScopedTimeDistributionUpdater time(&_stats.total_time);
                     time.alsoUpdate(&_stats.notify_time));

    const BooleanVariable variable = literal.variable();
    for (const unsigned int& index : _group.watch(variable)) {
        const std::unique_ptr<CosyStatus>& status = _statuses[index];
        if (status->state() != INACTIVE)
            status->updateNotify(literal);

        if (status->state() == REDUCER) {
            status->generateESBP(literal.variable(), injector);
            break;
        }
    }
}

void CosyManager::updateCancel(const Literal& literal) {
    IF_STATS_ENABLED(ScopedTimeDistributionUpdater time(&_stats.total_time);
                     time.alsoUpdate(&_stats.cancel_time));

    const BooleanVariable variable = literal.variable();
    for (const unsigned int& index : _group.watch(variable)) {
        const std::unique_ptr<CosyStatus>& status = _statuses[index];

        status->updateCancel(literal);
    }
}

void CosyManager::summarize() const {
    Printer::printStat("Variable Order", _order->variableModeString());
    Printer::printStat("Value Order", _order->valueModeString());
    Printer::printStat("Order", _order->preview());
}


}  // namespace cosy
