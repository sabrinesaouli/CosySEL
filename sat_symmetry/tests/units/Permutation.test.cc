
#include <gtest/gtest.h>
#include <memory>

#include "cosy/Permutation.h"

namespace cosy {

class PermutationTest : public testing::Test {
 protected:

    virtual void SetUp() {
        const int num_vars = 6;

        // (1 2) (3 -4 5) (-1 -2) (-3 4 -5)
        permutation = std::unique_ptr<Permutation>(new Permutation(num_vars));

        // Use implicit conversion of int to Literal
        permutation->addToCurrentCycle(1);
        permutation->addToCurrentCycle(2);
        permutation->closeCurrentCycle();

        permutation->addToCurrentCycle(3);
        permutation->addToCurrentCycle(-4);
        permutation->addToCurrentCycle(5);
        permutation->closeCurrentCycle();

        permutation->addToCurrentCycle(-1);
        permutation->addToCurrentCycle(-2);
        permutation->closeCurrentCycle();

        permutation->addToCurrentCycle(-3);
        permutation->addToCurrentCycle(4);
        permutation->addToCurrentCycle(-5);
        permutation->closeCurrentCycle();

    }

    std::unique_ptr<Permutation> permutation;
};


TEST_F(PermutationTest, numberOfCycles) {
    ASSERT_EQ(permutation->numberOfCycles(), static_cast<unsigned int>(4));
}

TEST_F(PermutationTest, imageOf) {
    Literal image = permutation->imageOf(1);
    ASSERT_EQ(image, 2);

    image = permutation->imageOf(2);
    ASSERT_EQ(image, 1);

    image = permutation->imageOf(3);
    ASSERT_EQ(image, -4);

    image = permutation->imageOf(-4);
    ASSERT_EQ(image, 5);

    image = permutation->imageOf(5);
    ASSERT_EQ(image, 3);

    image = permutation->imageOf(-1);
    ASSERT_EQ(image, -2);

    image = permutation->imageOf(-2);
    ASSERT_EQ(image, -1);

    image = permutation->imageOf(-3);
    ASSERT_EQ(image, 4);

    image = permutation->imageOf(4);
    ASSERT_EQ(image, -5);

    image = permutation->imageOf(-5);
    ASSERT_EQ(image, -3);
}

TEST_F(PermutationTest, inverseOf) {
    Literal image = permutation->inverseOf(1);
    ASSERT_EQ(image, 2);

    image = permutation->inverseOf(2);
    ASSERT_EQ(image, 1);

    image = permutation->inverseOf(3);
    ASSERT_EQ(image, 5);

    image = permutation->inverseOf(-4);
    ASSERT_EQ(image, 3);

    image = permutation->inverseOf(5);
    ASSERT_EQ(image, -4);

    image = permutation->inverseOf(-1);
    ASSERT_EQ(image, -2);

    image = permutation->inverseOf(-2);
    ASSERT_EQ(image, -1);

    image = permutation->inverseOf(-3);
    ASSERT_EQ(image, -5);

    image = permutation->inverseOf(4);
    ASSERT_EQ(image, -3);

    image = permutation->inverseOf(-5);
    ASSERT_EQ(image, 4);
}

TEST_F(PermutationTest, isTrivialImage) {
    ASSERT_TRUE(permutation->isTrivialImage(6));
    ASSERT_FALSE(permutation->isTrivialImage(3));
}

TEST_F(PermutationTest, isTrivialInverse) {
    ASSERT_TRUE(permutation->isTrivialInverse(6));
    ASSERT_FALSE(permutation->isTrivialInverse(3));
}


} // namespace cosy
