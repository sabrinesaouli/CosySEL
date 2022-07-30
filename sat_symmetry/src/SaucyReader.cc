// Copyright 2017 Hakan Metin - LIP6

#include "cosy/SaucyReader.h"

namespace {
static inline int graphNodeToInt(int node, int num_vars) {
    if (node <= num_vars)
        return node;
    if (node <= 2*num_vars)
        return num_vars - node;
    else
        return 0;  // represent clause
}

}  // namespace

namespace cosy {

bool SaucyReader::load(const std::string& symmetry_filename,
                       unsigned int num_vars, Group *group) {
    std::unique_ptr<Permutation> generator;
    StreamBuffer in(symmetry_filename);
    int parsed, cnf_int;
    Literal lit;

    CHECK_EQ(*in, '[');
    ++in;
    CHECK_EQ(*in, '\n');
    ++in;

    if (*in == ']')  // No generator exists
        return true;

    while (*in != '\n') {
        CHECK_EQ(*in, '(');
        generator = std::unique_ptr<Permutation>(new Permutation(num_vars));

        while (*in == '(') {
            ++in;
            parsed = in.readInt();
            cnf_int = graphNodeToInt(parsed, num_vars);
            if (cnf_int != 0) {
                lit = Literal(cnf_int);
                generator->addToCurrentCycle(lit);
            }

            while (*in != ')') {
                CHECK_EQ(*in, ',');
                ++in;
                parsed = in.readInt();
                cnf_int = graphNodeToInt(parsed, num_vars);
                if (cnf_int != 0) {
                    lit = Literal(cnf_int);
                    generator->addToCurrentCycle(lit);
                }
            }
            CHECK_EQ(*in, ')');
            generator->closeCurrentCycle();
            ++in;
        }
        group->addPermutation(std::move(generator));
        /* End of Generator line */
        if (*in == ',') {
            ++in;
            CHECK_EQ(*in, '\n');
            ++in;
        }
    }
    ++in;
    CHECK_EQ(*in, ']');

    return true;
}

}  // namespace cosy


/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
