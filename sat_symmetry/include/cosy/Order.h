// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_ORDER_H_
#define INCLUDE_COSY_ORDER_H_

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <memory>
#include <limits>
#include <string>


#include "cosy/Assignment.h"
#include "cosy/CNFModel.h"
#include "cosy/DisjointSets.h"
#include "cosy/Group.h"
#include "cosy/Literal.h"
#include "cosy/Orbits.h"

namespace cosy {

enum OrderMode {
    INCREASE,
    OCCURENCE,
    BREAKID,
    AUTO,
};

enum ValueMode {
    TRUE_LESS_FALSE,
    FALSE_LESS_TRUE,
};

class Order {
 public:
    explicit Order(unsigned int num_vars, ValueMode mode);
    virtual ~Order() {}

    unsigned int size() const { return _order.size(); }
    bool contains(const Literal& literal) const;

    const Literal leq(const Literal& a, const Literal& b) const;
    bool isMinimalValue(const Literal& literal,
                        const Assignment& assignment) const;

    bool isMaximalValue(const Literal& literal,
                        const Assignment& assignment) const;

    std::vector<Literal>::const_iterator begin() const {
        return _order.begin();
    }
    std::vector<Literal>::const_iterator end()   const {
        return _order.end();
    }

    ValueMode valueMode() const { return _valueMode; }

    std::string preview() const;
    std::string valueModeString() const;
    virtual std::string variableModeString() const = 0;

 protected:
    const unsigned int _num_vars;
    ValueMode _valueMode;
    std::vector<Literal> _order;
    std::unordered_map<Literal, unsigned int> _indexes;
    LiteralIndex _minimal, _maximal;

    void add(const Literal& literal);
};

/*----------------------------------------------------------------------------*/
class IncreaseOrder : public Order {
 public:
    explicit IncreaseOrder(unsigned int num_vars, ValueMode mode) :
        Order(num_vars, mode) {
        for (BooleanVariable var(0); var < num_vars; ++var)
            add(Literal(var, true));
    }
    ~IncreaseOrder() {}
    std::string variableModeString() const { return std::string("increase"); }

 private:
};
/*----------------------------------------------------------------------------*/
struct OrderLt {
    const std::vector<int64>& values;

    bool operator() (int i, int j) {
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OrderLt(const std::vector<int64>& v) :  values(v) {}
};

class OccurenceOrder : public Order {
 public:
    explicit OccurenceOrder(int64 num_vars, ValueMode mode,
                            const CNFModel& cnf_model_stats) :
        Order(num_vars, mode) {
        std::vector<int64> indexes;
        for (int64 i = 0; i < num_vars; ++i) {
            indexes.push_back(i);
        }

        std::sort(indexes.begin(), indexes.end(),
                  OrderLt(cnf_model_stats.occurences()));

        for (int64 i = 0; i < num_vars; ++i)
            add(Literal(BooleanVariable(indexes[i]), true));
    }
    ~OccurenceOrder() {}
    std::string variableModeString() const { return std::string("occurence"); }

 private:
};
/*----------------------------------------------------------------------------*/
class BreakIDOrder : public Order {
 public:
    explicit BreakIDOrder(int64 num_vars, ValueMode mode,
                            const Group& group) :
        Order(num_vars, mode) {
        std::vector<Permutation*> Q, filter;
        std::unordered_map<BooleanVariable, int64> occurence_generators;
        Orbits orbits;
        unsigned int largestOrbit;
        BooleanVariable next;

        for (const std::unique_ptr<Permutation>& perm : group.permutations())
            Q.push_back(perm.get());

        while (Q.size() > 0) {
            // Compute occurence in remain generators
            occurence_generators.clear();
            for (const Permutation* permutation : Q) {
                for (const Literal& literal : permutation->support())
                    if (literal.isPositive())
                        occurence_generators[literal.variable()]++;
            }
            occurence_generators[kNoBooleanVariable] =
                std::numeric_limits<int>::max();

            // Find next variable with less occurences in the largest orbit
            orbits.assign(Q);

            largestOrbit = 0;
            next = kNoBooleanVariable;

            for (const std::vector<BooleanVariable>& orbit : orbits) {
                if (orbit.size() < largestOrbit)
                    continue;
                for (const BooleanVariable& variable : orbit) {
                    if (occurence_generators[variable] == 0)
                        continue;
                    const int occ_v = occurence_generators[variable];
                    const int occ_n = occurence_generators[next];
                    if (next == kNoBooleanVariable ||
                        occ_v < occ_n || (occ_v == occ_n && variable < next)) {
                        next = variable;
                        largestOrbit = orbit.size();
                    }
                }
            }

            if (next == kNoBooleanVariable) {
                LOG(ERROR) << "Problem with generators ??";
                break;
            }

            add(Literal(next, true));

            // Remove permutation contains next
            filter.clear();
            const Literal next_literal = Literal(next, true);
            for (Permutation* permutation : Q)
                if (permutation->isTrivialImage(next_literal))
                    filter.push_back(permutation);

            Q = filter;
        }

        // Complete order
        for (BooleanVariable var(0); var < num_vars; ++var) {
            const Literal literal(var, true);
            if (!contains(literal))
                add(literal);
        }
    }
    ~BreakIDOrder() {}
    std::string variableModeString() const { return std::string("breakid"); }

 private:
};

}  // namespace cosy
#endif  // INCLUDE_COSY_ORDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
