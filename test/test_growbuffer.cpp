#include "growbuffer.hpp"
#include "test_common.hpp"

TEST(BUFFER, GROW) {
    GrowBuffer buf;
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE, buf.reservedSize());
    ASSERT_EQ(0, buf.size());

    buf.append('a');
    ASSERT_EQ(1, buf.size());
    ASSERT_EQ('a', buf[0]);
    ASSERT_EQ('a', *buf.data());

    buf.append('b');
    buf.append('\0');
    ASSERT_EQ(3, buf.size());
    ASSERT_EQ('a', buf[0]);
    ASSERT_EQ('b', buf[1]);
    ASSERT_EQ('\0', buf[2]);
    ASSERT_STREQ("ab", buf.data());
}

TEST(BUFFER, GROW2) {
    GrowBuffer buf;
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE, buf.reservedSize());
    ASSERT_EQ(0, buf.size());

    for (int i = 0; i < GROWBUFFER_DEFAULT_SIZE + 10; i++) {
        buf.append('0'+(i % 10));
    }

    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE + 10, buf.size());
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE*2, buf.reservedSize());

    for (int i = 0; i < GROWBUFFER_DEFAULT_SIZE + 10; i++) {
        ASSERT_EQ('0'+(i % 10), buf[i]);
    }
}

TEST(BUFFER, GROW3) {
    char buf2[GROWBUFFER_DEFAULT_SIZE*2];
    GrowBuffer buf;
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE, buf.reservedSize());
    ASSERT_EQ(0, buf.size());

    for (int i = 0; i < GROWBUFFER_DEFAULT_SIZE + 10; i++) {
        buf2[i] = '0'+(i % 10);
    }
    buf.append(buf2, GROWBUFFER_DEFAULT_SIZE + 10);

    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE + 10, buf.size());
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE*2, buf.reservedSize());

    for (int i = 0; i < GROWBUFFER_DEFAULT_SIZE + 10; i++) {
        ASSERT_EQ('0'+(i % 10), buf[i]);
    }

    buf.append(buf2, GROWBUFFER_DEFAULT_SIZE + 10);

    ASSERT_EQ((GROWBUFFER_DEFAULT_SIZE + 10)*2, buf.size());
    ASSERT_EQ(GROWBUFFER_DEFAULT_SIZE*3, buf.reservedSize());

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < GROWBUFFER_DEFAULT_SIZE + 10; i++) {
            ASSERT_EQ('0'+(i % 10), buf[i + j*(GROWBUFFER_DEFAULT_SIZE + 10)]);
        }
    }
}
