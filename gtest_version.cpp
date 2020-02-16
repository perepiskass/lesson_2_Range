//#include "lib.h"

#include <gtest/gtest.h>

TEST(sample_test_case, version_test)
{
    ASSERT_GT(1, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}