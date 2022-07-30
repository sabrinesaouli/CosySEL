#ifndef GlucoseLiteralAdapter_h
#define GlucoseLiteralAdapter_h

#include "cosy/LiteralAdapter.h"
#include "cosy/Literal.h"

#include "core/SolverTypes.h"

#include <iostream>

namespace Glucose {

class GlucoseLiteralAdapter : public cosy::LiteralAdapter<Glucose::Lit>
{
 public:
        GlucoseLiteralAdapter() {}
        ~GlucoseLiteralAdapter() {}

        virtual Glucose::Lit convertFrom(cosy::Literal l) {
            return mkLit(l.variable().value(), l.isNegative());
        }

        virtual cosy::Literal convertTo(Glucose::Lit lit) {
            return (sign(lit) ? -(var(lit) + 1) : var(lit) + 1);
        }
};

}

#endif
