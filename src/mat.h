#ifndef LINEAR_MAT_H
#define LINEAR_MAT_H
#include <cmath>
#include <algorithm>

namespace lnr {

    template <size_t M, size_t N = M>
    class mat {
        float* m_Values;
        size_t* m_MIdx;
        size_t* m_NIdx;
    public:

        inline static const mat ZERO = mat(0.f);

        void initIndexer() const {
            for (std::size_t m = 0; m < M; ++m) m_MIdx[m] = m;
            for (std::size_t n = 0; n < N; ++n) m_NIdx[n] = n;
        }

        explicit mat(float value = 0.f) {
            m_Values = new float[M * N]{value};
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIndexer();
        }

        explicit mat(const float (&values)[M * N]) {
            m_Values = new float[M * N];
            std::copy(std::begin(values), std::end(values) + M * N, m_Values);
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIndexer();
        }

        template<typename ...Args>
        constexpr mat(Args... values) {
            static_assert(sizeof...(Args) == M * N, "Size mismatch");
            m_Values = new float[M * N];
            std::size_t i = 0;
            ((m_Values[i++] = values), ...);
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIndexer();
        }

        mat(const mat &m) {
            m_Values = new float[M * N];
            std::copy(m.m_Values, m.m_Values + N * M, m_Values);
            m_MIdx = new size_t[M];
            std::copy(m.m_MIdx, m.m_MIdx + M, m_MIdx);
            m_NIdx = new size_t[N];
            std::copy(m.m_NIdx, m.m_NIdx + N, m_NIdx);
        }

        mat(mat &&m) noexcept {
            m_Values = m.m_Values;
            m.m_Values = nullptr;
            m_MIdx = m.m_MIdx;
            m.m_MIdx = nullptr;
            m_NIdx = m.m_NIdx;
            m.m_NIdx = nullptr;
        }

        virtual ~mat() {
            delete[] m_Values;
            delete[] m_MIdx;
            delete[] m_NIdx;
        }

        bool operator==(const mat &other) const {
            if (this == &other) return true;
            for (size_t m = 0; m < M; ++m)
                for (size_t n = 0; n < N; ++n)
                    if (operator()(m, n) != other(m, n)) return false;
            return true;
        }

        bool operator!=(const mat &other) const {
            return !(*this == other);
        }

        mat& operator=(const mat &other) {
            if (this == &other) return *this;
            std::copy(other.m_Values, other.m_Values + N * M, m_Values);
            std::copy(other.m_MIdx, other.m_MIdx + M, m_MIdx);
            std::copy(other.m_NIdx, other.m_NIdx + N, m_NIdx);
            return *this;
        }

        mat& operator=(mat &&other) noexcept {
            if (this == &other) return *this;
            m_Values = other.m_Values;
            other.m_Values = nullptr;
            return *this;
        }

        mat operator-() const {
            return *this * -1;
        }

        mat& operator+=(const mat &other) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] += other.m_Values[i];
            return *this;
        }

        mat& operator-=(const mat &other) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] -= other.m_Values[i];
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

        mat operator+(const mat &other) const {
            return mat(*this) += other;
        }

        mat operator-(const mat &other) const {
            return mat(*this) -= other;
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
            return m_Values[m_MIdx[m] * N + m_NIdx[n]];
        }

        float& operator()(const size_t m, const size_t n) {
            return m_Values[m_MIdx[m] * N + m_NIdx[n]];
        }


#if __cplusplus >= 202302L
        float operator[](const size_t m, const size_t n) const {
            return operator()(other, n);
        }

        float& operator[](const size_t m, const size_t n) {
            return operator()(other, n);
#endif

        mat& swapM(const size_t m0, const size_t m) {
            if (m0 == m) return *this;
            std::swap(m_MIdx[m0], m_MIdx[m]);
            return *this;
        }

        mat& swapN(const size_t n0, const size_t n) {
            if (n0 == n) return *this;
            std::swap(m_NIdx[n0], m_NIdx[n]);
            return *this;
        }

        [[nodiscard]] float det() const {
            static_assert(M == N, "Matrix is not square");
            mat plu = *this;
            float ret = 1;
            for (size_t p = 0; p < M; ++p) {
                auto p_ = p;
                while (p_ < M && plu(p_, p) == 0) ++p_;
                if (p_ == M) return 0;
                if (p_ != p) {
                    plu.swapM(p_, p);
                    ret = -ret;
                }
                auto pivot = plu(p, p);
                ret *= pivot;
                for (size_t m = p + 1; m < M; ++m) {
                    auto f = plu(m, p) / pivot;
                    plu(m, p) = f;
                    for (size_t n = p + 1; n < N; ++n) {
                        plu(m, n) -= f * plu(p, n);
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
