
#include <gtest/gtest.h>

#include "cosy/StreamBuffer.h"

namespace cosy {

// TEST(StreamBufferTest, fileNotExists) {
//     ASSERT_DEATH(StreamBuffer stream("file_not_exists"),
//                  ".*Cannot open file.*");
// }

TEST(StreamBufferTest, operatorStar) {
    StreamBuffer stream("tests/resources/one.cnf");

    ASSERT_EQ(*stream, 'p');
}

TEST(StreamBufferTest, operatorPlusPlus) {
    StreamBuffer stream("tests/resources/one.cnf");

    ASSERT_EQ(*stream, 'p'); ++stream;
    ++stream; // ' '
    ++stream; // ' '
    ASSERT_EQ(*stream, 'c'); ++stream;
    ASSERT_EQ(*stream, 'n'); ++stream;
    ASSERT_EQ(*stream, 'f'); ++stream;
    ASSERT_EQ(*stream, ' '); ++stream;
    ASSERT_EQ(*stream, '3'); ++stream;
    ASSERT_EQ(*stream, ' '); ++stream;
    ASSERT_EQ(*stream, '1'); ++stream;
    ASSERT_EQ(*stream, '\n'); ++stream;
}

TEST(StreamBufferTest, skipWhitespaces) {
    StreamBuffer stream("tests/resources/one.cnf");

    stream.skipWhiteSpaces();
    ASSERT_EQ(*stream, 'p'); ++stream;
    stream.skipWhiteSpaces();
    ASSERT_EQ(*stream, 'c');
}

TEST(StreamBufferTest, skipLine) {
    StreamBuffer stream("tests/resources/one.cnf");

    ASSERT_EQ(*stream, 'p');
    stream.skipLine();
    ASSERT_EQ(*stream, '1');

    stream.skipLine();
    stream.skipLine();
    stream.skipLine();

    ASSERT_EQ(*stream, '\0');
}

TEST(StreamBufferTest, readInt) {
    int value;
    StreamBuffer stream("tests/resources/one.cnf");

    stream.skipLine();

    value = stream.readInt();
    ASSERT_EQ(value, 1);
    ASSERT_EQ(*stream, ' '); ++stream;

    value = stream.readInt();
    ASSERT_EQ(value, -2);
    ASSERT_EQ(*stream, ' '); ++stream;

    value = stream.readInt();
    ASSERT_EQ(value, 3);
    ASSERT_EQ(*stream, ' '); ++stream;
}

} // namespace cosy
