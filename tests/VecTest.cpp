#include "gtest/gtest.h"
#include "vec.h"
#include <cmath>

using namespace lnr;

TEST(VecTest, DotProduct) {
    const vec<2> v1{2, 3}, v2{5, 7};
    EXPECT_EQ((v1.t() * v2)(0, 0), 31);
}

TEST(VecTest, MatrixMultiplication) {
    mat<2> mat2x2{1, 2, 3, 4};
    vec<2> vec2{-1, 1};
    EXPECT_EQ(mat2x2 * vec2, vec<2>(1, 1));
}

TEST(VecTest, AngleBetweenTwoVectors) {
    const vec<2> v1{1, 0}, v2{0, 1};
    EXPECT_FLOAT_EQ(angle(v1, v2), M_PI / 2);
}