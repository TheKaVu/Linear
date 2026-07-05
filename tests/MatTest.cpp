#include "gtest/gtest.h"
#include "mat.h"
#include "vec.h"

using namespace lnr;

TEST(MatTest, Constructors) {
    mat<3> m1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i = 1;
    for (std::size_t m = 0; m < 3; ++m)
        for (std::size_t n = 0; n < 3; ++n)
            EXPECT_EQ(m1(m, n), i++);
}

TEST(MatTest, Scaling) {
    mat<3> m1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto m2 = 2 * m1;
    EXPECT_EQ(m2, mat<3>(2, 4, 6, 8, 10, 12, 14, 16, 18));
    EXPECT_EQ(m2 / 2, m1);
}

TEST(MatTest, ArithmeticOperators) {
    mat<3, 2> mat3x2{1, 2, 3, 4, 5, 6};
    mat<2, 3> mat2x3{1, 2, 3, 4, 5, 6};

    EXPECT_EQ(mat3x2 + mat3x2, mat3x2 * 2);

    auto substract = mat2x3 - mat2x3;
    EXPECT_EQ(substract, (mat<2, 3>::ZERO));

    EXPECT_EQ(-mat3x2, (mat<3, 2>(-1, -2, -3, -4, -5, -6)));

    vec<3> m = mat3x2 * vec<2>();

    EXPECT_EQ(mat3x2 * mat2x3, mat<3>(9, 12, 15, 19, 26, 33, 29, 40, 51));

    EXPECT_EQ(mat2x3 * mat3x2, mat<2>(22, 28, 49, 64));
}

TEST(MatTest, Transposition) {
    mat<3,2> mat3x2{1, 2, 3, 4, 5, 6};
    auto m = mat3x2.t();
    mat<2, 3> mat2x3transposed{1, 3, 5, 2, 4, 6};
    EXPECT_EQ(mat3x2.t(), mat2x3transposed);
}

TEST(MatTest, Submatrix) {
    mat<2> m = mat<4>::idt().sub<2, 2>(1, 1);
    EXPECT_EQ(m, mat<2>::idt());
}

TEST(MatTest, Minor) {
    mat<3> m {1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto minor = m.minor(1, 1);
    EXPECT_EQ(minor, mat<2>(1, 3, 7, 9));
}

TEST(MatTest, IdentityMatrix) {
    const mat<3> m1{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    EXPECT_EQ(m1 * mat<3>::idt(), m1);
    EXPECT_EQ(mat<3>::idt().t(), mat<3>::idt());
}

TEST(MatTest, Interpolation) {
    const mat<2> m1 {0, 2, 2, 0}, m2 {0, 4, 0, 0};
    EXPECT_EQ(lerp(m1, m2, 0.5), mat<2>(0, 3, 1, 0));
}

TEST(MatTest, Determinant) {
    mat<4> m{
        1, 3, 4, 0,
        -2, -5, 10, 2,
        -1, -2, 2, 3,
        -1, -2, -3, 1
    };

    EXPECT_FLOAT_EQ(m.det(), 29);
}