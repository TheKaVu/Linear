#ifndef LINEAR_MAT_H
#define LINEAR_MAT_H
#include <cmath>
#include <algorithm>
#include "kernel/mat_mul.hpp"
#include "kernel/mat_det.hpp"

namespace lnr {

    template <size_t M_, size_t N_ = M_>
    class mat {
        float* m_Values;

    public:
        static constexpr size_t M = M_;
        static constexpr size_t N = N_;

        inline static const mat ZERO = {};

        explicit mat(float value = 0.f) {
            m_Values = new float[M_ * N_]{value};
        }

        explicit mat(const float (&values)[M * N]) {
            m_Values = new float[M_ * N_];
            std::copy(std::begin(values), std::end(values) + M * N, m_Values);
        }

        template<typename ...Args>
        constexpr mat(Args... values) {
            static_assert(sizeof...(Args) == M * N, "Size mismatch");
            m_Values = new float[M_ * N_];
            std::size_t i = 0;
            ((m_Values[i++] = values), ...);
        }

        mat(const mat &m) {
            m_Values = new float[M_ * N_];
            std::copy(m.m_Values, m.m_Values + N * M, m_Values);
        }

        mat(mat &&m) noexcept {
            m_Values = m.m_Values;
            m.m_Values = nullptr;
        }

        virtual ~mat() {
            delete[] m_Values;
        }

        bool operator==(const mat &m) const {
            return this == &m || std::equal(m_Values, m_Values + M * N, m.m_Values);
        }

        bool operator!=(const mat &m) const {
            return !(*this == m);
        }

        mat& operator=(const mat &m) {
            if (this == &m) return *this;
            std::copy(m.m_Values, m.m_Values + N * M, m_Values);
            return *this;
        }

        mat& operator=(mat &&m) noexcept {
            if (this == &m) return *this;
            m_Values = m.m_Values;
            m.m_Values = nullptr;
            return *this;
        }

        mat operator-() const {
            return *this * -1;
        }

        mat& operator+=(const mat &m) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] += m.m_Values[i];
            return *this;
        }

        mat& operator-=(const mat &m) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] -= m.m_Values[i];
            return *this;
        }

        mat& operator*=(const float f) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] *= f;
            return *this;
        }

        mat& operator/=(const float f) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] /= f;
            return *this;
        }

        mat operator+(const mat &m) const {
            return mat(*this) += m;
        }

        mat operator-(const mat &m) const {
            return mat(*this) -= m;
        }

        mat operator*(const float f) const {
            return mat(*this) *= f;
        }

        mat operator/(const float f) const {
            return mat(*this) /= f;
        }

        template<size_t N_Other>
        mat<M, N_Other> operator*(const mat<N, N_Other> &other) const {
            mat<M, N_Other> ret;
            for (std::size_t m = 0; m < M; ++m) {
                for (std::size_t n = 0; n < N_Other; ++n) {
                    for (std::size_t o = 0; o < N; ++o) {
                        ret(m, n) += operator()(m, o) * other(o, n);
                    }
                }
            }
            return ret;
        }

        float operator()(const size_t m, const size_t n) const {
            return m_Values[m * N + n];
        }

        float& operator()(const size_t m, const size_t n) {
            return m_Values[m * N + n];
        }


#if __cplusplus >= 202302L
        float operator[](const size_t m, const size_t n) const {
            return m_Values[m * N + n];
        }

        float& operator[](const size_t m, const size_t n) {
            return m_Values[m * N + n];
        }
#endif

        [[nodiscard]] float det() const {
            static_assert(M == N, "Matrix is not square");
            mat temp = *this;
            float ret = 1;
            for (size_t i = 0; i < M; ++i) {
                float v0 = temp(i, i);
                if (v0 == 0) return 0;
                ret *= v0;
                for (size_t m = i + 1; m < M; ++m) {
                    float f = temp(m, i) / v0;
                    for (size_t n = i; n < N; ++n) {
                        temp(m, n) -= f * temp(i, n);
                    }
                }
            }
            return ret;
        }

        [[nodiscard]] mat<M - 1, N - 1> minor(const size_t m0, const size_t n0) const {
            mat<M - 1, N - 1> ret;
            for (size_t m = 0; m < M - 1; ++m) {
                for (size_t n = 0; n < N - 1; ++n) {
                    ret(m, n) = operator()(m + (m < m0 ? 0 : 1), n + (n < n0 ? 0 : 1));
                }
            }
            return ret;
        }

        template <std::size_t M_Sub, std::size_t N_Sub>
        [[nodiscard]] mat<M_Sub, N_Sub> sub(const size_t m0 = 0, const size_t n0 = 0) const {
            mat<M_Sub, N_Sub> ret;
            for (size_t m = 0; m < M_Sub; ++m ) {
                for (size_t n = 0; n < N_Sub; ++n) {
                    ret(m, n) = operator()(m + m0, n + n0);
                }
            }
            return ret;
        }

        [[nodiscard]] mat<N, M> t() const {
            mat<N, M> ret;
            for (size_t m = 0; m < M; ++m) {
                for (size_t n = 0; n < N; ++n) {
                    ret(n, m) = operator()(m, n);
                }
            }
            return ret;
        }

        [[nodiscard]] mat clamp(float minVal, float maxVal) {
            mat<M, N> ret;
            for (size_t m = 0; m < M; ++m) {
                ret = std::min(std::max(minVal, ret(m)), maxVal);
            }
            return ret;
        }

        [[nodiscard]] float trace() const {
            static_assert(M == N, "Matrix is not square");
            float ret = 0;
            for (size_t m = 0; m < M; ++m) {
                ret += operator()(m, m);
            }
            return ret;
        }

        static mat idt() {
            static_assert(M == N, "Matrix is not square");
            mat m;
            for (std::size_t i = 0; i < M; ++i) {
                m(i, i) = 1;
            }
            return m;
        }
    };

    // Global operators

    template <std::size_t M, std::size_t N>
    mat<M, N> operator * (float f, const mat<M, N> &v) {
        return mat<M, N>(v) *= f;
    }

    template <std::size_t M, std::size_t N>
    mat<M, N> lerp(const mat<M, N> &min, const mat<M, N> &max, float f) {
        return min * (1 - f) + max * f;
    }

}

#endif //LINEAR_MAT_H
