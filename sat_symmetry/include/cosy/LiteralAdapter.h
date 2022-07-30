// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_LITERALADAPTER_H_
#define INCLUDE_COSY_LITERALADAPTER_H_

#include "cosy/Literal.h"

namespace cosy {

template<class T>
class LiteralAdapter {
 public:
        LiteralAdapter() {}
        virtual ~LiteralAdapter() {}

        virtual T convertFrom(cosy::Literal l) = 0;
        virtual cosy::Literal convertTo(T from) = 0;
};


template<>
class LiteralAdapter<cosy::Literal> {
    virtual cosy::Literal convertFrom(cosy::Literal l) {
        return l;
    }
    virtual cosy::Literal convertTo(cosy::Literal from) {
        return from;
    }
};

}  // namespace cosy

#endif  // INCLUDE_COSY_LITERALADAPTER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
