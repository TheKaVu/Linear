#ifndef LINEAR_VEC_H
#define LINEAR_VEC_H
#include <cmath>
#include <algorithm>
#include "mat.h"//meow meow meow meow

namespace lnr {
    template <std::size_t N_>
    class vec : public mat<N_, 1> {

    public:
        static constexpr std::size_t N = N_;

        vec() : mat<N, 1>() {}

        vec(const float (&values)[N]) : mat<N, 1>(values) {}

        template<typename ...Args>
        constexpr vec(Args... values) : mat<N, 1>(values...) {}

        vec(const vec &v) : mat<N, 1>(v) {}

        vec(vec &&v) noexcept : mat<N, 1>(std::move(v)) {}

        ~vec() override = default;

        float operator[](const std::size_t i) const {
            return mat<N, 1>::operator()(i, 0);
        }

        [[nodiscard]] float mag2() const {
            float sum_s = 0;
            for (std::size_t i = 0; i < N; ++i) {
                sum_s += operator[](i) * operator[](i);
            }
            return sum_s;
        }

        [[nodiscard]] float mag() const {
            return sqrtf(mag2());
        }

        [[nodiscard]] vec norm() const {
            return *this / mag();
        }

        static vec versor(const std::size_t i) {
            vec ret;
            ret[i] = 1;
            return ret;
        }
    };

    template <std::size_t N>
    float angle(const vec<N> &a, const vec<N> &b) {
        return acosf((a.t() * b)(0, 0) / (a.mag() * b.mag()));
    }

    inline vec<3> cross(const vec<3> &a, const vec<3> &b) {
        return {
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]
        };
    }
}

#endif //LINEAR_VEC_H