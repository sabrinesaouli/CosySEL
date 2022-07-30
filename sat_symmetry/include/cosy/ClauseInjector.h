// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_CLAUSEINJECTOR_H_
#define INCLUDE_COSY_CLAUSEINJECTOR_H_

#include <deque>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <utility>

#include "cosy/Clause.h"
#include "cosy/Macros.h"
#include "cosy/Literal.h"
#include "cosy/Logging.h"
#include "cosy/Stats.h"

namespace cosy {


class Injector {
 public:
    Injector() {}
    ~Injector() {}

    void addClause(BooleanVariable cause, std::vector<Literal>&& literals) {
        if (cause == kNoBooleanVariable || _clauses[cause].empty())
            _clauses[cause].push_back(std::move(literals));
    }

    void removeClause(BooleanVariable cause) {
        _clauses[cause].clear();
    }

    bool hasClause(BooleanVariable cause) const {
        if (_clauses.find(cause) == _clauses.end())
            return false;

        return _clauses.at(cause).size() > 0;
    }
    std::vector<Literal> getClause(BooleanVariable cause) {
        std::vector<Literal> literals;

        CHECK_EQ(hasClause(cause), true);

        literals = std::move(_clauses[cause].back());
        _clauses[cause].pop_back();

        return literals;
    }

 private:
    std::unordered_map<BooleanVariable,
                       std::vector<std::vector<Literal>>> _clauses;
};


class ClauseInjector {
 public:
    enum Type {
        UNITS,
        ESBP,
        ESBP_FORCING,
        NR_TYPES
    };

    ClauseInjector();
    ~ClauseInjector();


    void addClause(Type type, BooleanVariable cause,
                   std::vector<Literal>&& literals);
    void removeClause(Type type, BooleanVariable cause);
    bool hasClause(Type type, BooleanVariable cause) const;
    std::vector<Literal> getClause(Type type, BooleanVariable cause);

    void removeClause(BooleanVariable cause);

    void printStats() const { _stats.print(); }

 private:
    std::vector<Injector> _injectors;

    struct Stats : StatsGroup {
        Stats() : StatsGroup("Clause Injector"),
                  units("Number of Units", this),
                  esbp("Number of ESBP", this),
                  esbp_forcing("Number of ESBP Forcing", this) {}

        CounterStat units;
        CounterStat esbp;
        CounterStat esbp_forcing;
    };
    Stats _stats;

    DISALLOW_COPY_AND_ASSIGN(ClauseInjector);
};

}  // namespace cosy

#endif  // INCLUDE_COSY_CLAUSEINJECTOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
