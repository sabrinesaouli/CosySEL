// Copyright 2017 Hakan Metin - LIP6

#include "cosy/BreakIDReader.h"

namespace cosy {

bool BreakIDReader::load(const std::string& symmetry_filename,
                       unsigned int num_vars, Group *group) {
    std::unique_ptr<Permutation> generator;
    StreamBuffer in(symmetry_filename);
    int parsed;
    Literal lit;
    unsigned int num_rows, num_columns;
    std::vector< std::vector<Literal> > matrix;

    while (*in) {
        if (*in == '(') {
            CHECK_EQ(*in, '(');
            generator = std::unique_ptr<Permutation>(new Permutation(num_vars));

            while (*in && *in != '\n') {
                CHECK_EQ(*in, '(');
                ++in;
                CHECK_EQ(*in, ' ');
                ++in;
                parsed = in.readInt();
                CHECK_EQ(*in, ' ');
                ++in;
                generator->addToCurrentCycle(parsed);

                while (*in != ')') {
                    parsed = in.readInt();
                    CHECK_EQ(*in, ' ');
                    ++in;
                    generator->addToCurrentCycle(parsed);
                }
                CHECK_EQ(*in, ')');
                generator->closeCurrentCycle();
                ++in;
                CHECK_EQ(*in, ' ');
                ++in;
            }
            ++in;
            group->addPermutation(std::move(generator));
        } else if (*in == 'r') {
            ++in; CHECK_EQ(*in, 'o');
            ++in; CHECK_EQ(*in, 'w');
            ++in; CHECK_EQ(*in, 's');
            ++in; CHECK_EQ(*in, ' ');

            num_rows = in.readInt();

            ++in; CHECK_EQ(*in, 'c');
            ++in; CHECK_EQ(*in, 'o');
            ++in; CHECK_EQ(*in, 'l');
            ++in; CHECK_EQ(*in, 'u');
            ++in; CHECK_EQ(*in, 'm');
            ++in; CHECK_EQ(*in, 'n');
            ++in; CHECK_EQ(*in, 's');
            ++in; CHECK_EQ(*in, ' ');

            num_columns = in.readInt();
            matrix.clear();
            matrix.resize(num_rows);
            for (unsigned int i=0; i<num_rows; i++) {
                for (unsigned int j=0; j<num_columns; j++) {
                    parsed = in.readInt();
                    matrix[i].push_back(parsed);
                    CHECK_EQ(*in, ' ');
                    ++in;
                }
                CHECK_EQ(*in, '\n');
                ++in;
            }

            /* Use Matrix */
            for (unsigned int i=0; i<num_rows; i++) {
                for (unsigned int j=i+1; j<num_rows; j++) {
                    generator = std::unique_ptr<Permutation>(new Permutation(num_vars));
                    for (unsigned int k=0; k<num_columns; k++) {
                        generator->addToCurrentCycle(matrix[i][k]);
                        generator->addToCurrentCycle(matrix[j][k]);
                        generator->closeCurrentCycle();
                    }
                    group->addPermutation(std::move(generator));
                }
            }
        }
    }

    return true;
}


}  // namespace cosy


/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
