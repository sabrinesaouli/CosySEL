// Copyright 2017 Hakan Metin - LIP6

#include "cosy/Order.h"

namespace cosy {

Order::Order(unsigned int num_vars, ValueMode mode)  :
    _num_vars(num_vars),
    _valueMode(mode) {
    if (mode == TRUE_LESS_FALSE) {
        _minimal = kTrueLiteralIndex;
        _maximal = kFalseLiteralIndex;
    } else if (mode == FALSE_LESS_TRUE) {
        _minimal = kFalseLiteralIndex;
        _maximal = kTrueLiteralIndex;
    } else {
        CHECK(false);  // must be one of above cases
    }
}

void Order::add(const Literal& literal) {
    CHECK(!contains(literal));
    const unsigned int sz = _order.size();
    _indexes[literal] = sz;
    _indexes[literal.negated()] = sz;
    _order.push_back(literal);
}

bool Order::contains(const Literal& literal) const {
    return _indexes.find(literal) != _indexes.end();
}

const Literal Order::leq(const Literal& a, const Literal& b) const {
    // <= is really important, on inverting -1, 1 we must return
    // the positive value because our order is only positive element
    return _indexes.at(a) <= _indexes.at(b) ? a : b;
}

bool
Order::isMinimalValue(const Literal& lit, const Assignment& assignment) const {
    return (_minimal == kTrueLiteralIndex && assignment.literalIsTrue(lit)) ||
        (_minimal == kFalseLiteralIndex && assignment.literalIsFalse(lit));
}

bool
Order::isMaximalValue(const Literal& lit, const Assignment& assignment) const {
    return (_maximal == kTrueLiteralIndex && assignment.literalIsTrue(lit)) ||
        (_maximal == kFalseLiteralIndex && assignment.literalIsFalse(lit));
}

std::string Order::preview() const {
    const int korderElement = 10;
    std::string order = "";
    int i = 0;
    for (const Literal& literal : _order) {
        order += std::to_string(literal.signedValue()) + " ";
        if (++i == korderElement) {
            order += "...";
            break;
        }
    }

    // for (int j = _order.size() - korderElement -1; j >= 0; ++j) {
    //     const Literal& literal = _order[j];
    //     order +=  std::to_string(literal.signedValue()) + " ";
    // }

    return order;
}

std::string Order::valueModeString() const {
    CHECK_NE(_minimal, _maximal);
    if (_minimal == kFalseLiteralIndex) return std::string("False < True");
    if (_minimal == kTrueLiteralIndex)  return std::string("True  < False");
    return std::string("UNKNOWN");
}


}  // namespace cosy

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
