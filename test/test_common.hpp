#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "gtest/gtest.h"

#define ASSERT_MEMEQ(val1, len1, val2, len2) \
    do {\
        ASSERT_EQ(len1, len2);                  \
        ASSERT_EQ(0, memcmp(val1, val2, len1));  \
    } while(0)

#endif /* TEST_COMMON_H */
