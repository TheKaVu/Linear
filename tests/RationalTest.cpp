#include "gtest/gtest.h"
#include "rational.h"

using namespace lnr;

TEST(RationalTest, EqualityAndInternalSimplification) {
    rational a{1, 2}, b = a, c;
    c = a;
    EXPECT_EQ(a, b);
    EXPECT_EQ(a, c);
    EXPECT_EQ(rational(-3), rational(3, -1));
    EXPECT_EQ(rational(4, 6), rational(2, 3));
    EXPECT_EQ(-(-rational(3, 4)), rational(3, 4));
}

TEST(RationalTest, Comparison) {
    EXPECT_TRUE(rational(1, 2) < rational(2, 3));
    EXPECT_TRUE(rational(-1, 2) < rational(1, 2));
    EXPECT_TRUE(rational(-3, 4) < rational(-2, 3));
}

TEST(RationalTest, Arithmetic) {
    rational a = 3, b = 4;
    EXPECT_EQ(a + b, rational(7));
    EXPECT_EQ(a - b, rational(-1));
    EXPECT_EQ(a * b, rational(12));
    EXPECT_EQ(a / b, rational(3, 4));
}

TEST(RationalTest, ImplicitConversion) {
    rational a{1, 4};
    EXPECT_DOUBLE_EQ(a, 0.25);
    EXPECT_FLOAT_EQ(a, 0.25f);
}