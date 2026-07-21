#ifndef LINEAR_MAT_H
#define LINEAR_MAT_H
#include <cmath>
#include <algorithm>

namespace lnr {
    /// Represents a M by N matrix.
    /// @tparam M Number of rows (height).
    /// @tparam N Number of columns (width).
    template <size_t M, size_t N = M>
    class mat {
        float* m_Values;
        size_t* m_MIdx;
        size_t* m_NIdx;

        void initIdx() const {
            for (std::size_t m = 0; m < M; ++m) m_MIdx[m] = m;
            for (std::size_t n = 0; n < N; ++n) m_NIdx[n] = n;
        }

    public:

        /// A matrix filled with zeros.
        inline static const mat ZERO = mat(0.f);

        /// Creates new matrix filled with given value.
        /// @param value Value to fill every cell with.
        explicit mat(float value = 0.f) {
            m_Values = new float[M * N]{value};
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIdx();
        }

        /// Creates new matrix from a static array.
        /// @param values Input array.
        explicit mat(const float (&values)[M * N]) {
            m_Values = new float[M * N];
            std::copy(std::begin(values), std::end(values) + M * N, m_Values);
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIdx();
        }

        /// Creates new matrix with given values in cells, row by row.
        /// @param values Values to put in cells.
        template<typename ...Args>
        constexpr mat(Args... values) {
            static_assert(sizeof...(Args) == M * N, "Size mismatch");
            m_Values = new float[M * N];
            std::size_t i = 0;
            ((m_Values[i++] = values), ...);
            m_MIdx = new size_t[M];
            m_NIdx = new size_t[N];
            initIdx();
        }

        /// Creates new matrix by copying other one.
        /// @param other Matrix to copy.
        mat(const mat &other) {
            m_Values = new float[M * N];
            std::copy(other.m_Values, other.m_Values + N * M, m_Values);
            m_MIdx = new size_t[M];
            std::copy(other.m_MIdx, other.m_MIdx + M, m_MIdx);
            m_NIdx = new size_t[N];
            std::copy(other.m_NIdx, other.m_NIdx + N, m_NIdx);
        }

        /// Creates new matrix by moving other one.
        /// @param other Matrix ot move.
        mat(mat &&other) noexcept {
            m_Values = other.m_Values;
            other.m_Values = nullptr;
            m_MIdx = other.m_MIdx;
            other.m_MIdx = nullptr;
            m_NIdx = other.m_NIdx;
            other.m_NIdx = nullptr;
        }

        /// Destroys this matrix object.
        virtual ~mat() {
            delete[] m_Values;
            delete[] m_MIdx;
            delete[] m_NIdx;
        }

        /// Checks equality to given matrix.
        /// @param other Matrix to compare.
        /// @return @c true if matrices are equal, @c false otherwise.
        bool operator==(const mat &other) const {
            if (this == &other) return true;
            for (size_t m = 0; m < M; ++m)
                for (size_t n = 0; n < N; ++n)
                    if (operator()(m, n) != other(m, n)) return false;
            return true;
        }

        /// Checks inequality to given matrix.
        /// @param other Matrix to compare.
        /// @return @c true if matrices are not equal, @c false otherwise.
        bool operator!=(const mat &other) const {
            return !(*this == other);
        }

        /// Copies given matrix to this one.
        /// @param other Matrix to copy.
        /// @return This matrix.
        mat& operator=(const mat &other) {
            if (this == &other) return *this;
            std::copy(other.m_Values, other.m_Values + N * M, m_Values);
            std::copy(other.m_MIdx, other.m_MIdx + M, m_MIdx);
            std::copy(other.m_NIdx, other.m_NIdx + N, m_NIdx);
            return *this;
        }

        /// Moves given matrix to this one.
        /// @param other Matrix to move.
        /// @return This matrix.
        mat& operator=(mat &&other) noexcept {
            if (this == &other) return *this;
            m_Values = other.m_Values;
            other.m_Values = nullptr;
            return *this;
        }

        /// Negates all cells of this matrix.
        /// @return Matrix with negated values.
        mat operator-() const {
            return *this * -1;
        }

        /// Adds a matrix to this one.
        /// @param other Matrix to add.
        /// @return This matrix.
        mat& operator+=(const mat &other) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] += other.m_Values[i];
            return *this;
        }

        /// Subtracts a matrix from this one.
        /// @param other Matrix to subtract.
        /// @return This matrix.
        mat& operator-=(const mat &other) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] -= other.m_Values[i];
            return *this;
        }

        /// Multiplies this matrix by scalar.
        /// @param f Scalar.
        /// @return This matrix.
        mat& operator*=(const float f) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] *= f;
            return *this;
        }

        /// Divides this matrix by scalar.
        /// @param f Scalar.
        /// @return This matrix.
        mat& operator/=(const float f) {
            for (std::size_t i = 0; i < M * N; ++i)
                m_Values[i] /= f;
            return *this;
        }

        /// Adds two matrices.
        /// @param other Matrix to add.
        /// @return Result of addition
        mat operator+(const mat &other) const {
            return mat(*this) += other;
        }

        /// Subtracts two matrices.
        /// @param other Matrix to subtract.
        /// @return Result of subtraction.
        mat operator-(const mat &other) const {
            return mat(*this) -= other;
        }

        /// Multiplies matrix by scalar.
        /// @param f Scalar.
        /// @return Scaled matrix.
        mat operator*(const float f) const {
            return mat(*this) *= f;
        }

        /// Divides matrix by scalar.
        /// @param f Scalar.
        /// @return Scaled matrix.
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

        /// Gets the value of matrix cell.
        /// @param m Row index.
        /// @param n Column index.
        /// @return Value of respective cell.
        float operator()(const size_t m, const size_t n) const {
            return m_Values[m_MIdx[m] * N + m_NIdx[n]];
        }

        /// Accesses the value of matrix cell.
        /// @param m Row index.
        /// @param n Column index.
        /// @return Reference to value of respective cell.
        float& operator()(const size_t m, const size_t n) {
            return m_Values[m_MIdx[m] * N + m_NIdx[n]];
        }


#if __cplusplus >= 202302L
        /// Gets the value of matrix cell.
        /// @param m Row index.
        /// @param n Column index.
        /// @return Value of respective cell.
        float operator[](const size_t m, const size_t n) const {
            return operator()(other, n);
        }

        /// Accesses the value of matrix cell.
        /// @param m Row index.
        /// @param n Column index.
        /// @return Reference to value of respective cell.
        float& operator[](const size_t m, const size_t n) {
            return operator()(other, n);
        }
#endif

        /// Swaps two given rows of this matrix.
        /// @param m1 First row.
        /// @param m2 Second row.
        /// @return This matrix.
        mat& swapM(const size_t m1, const size_t m2) {
            if (m1 == m2) return *this;
            std::swap(m_MIdx[m1], m_MIdx[m2]);
            return *this;
        }

        /// Swaps two given columns of this matrix.
        /// @param n1 First column.
        /// @param n2 Second column.
        /// @return This matrix.
        mat& swapN(const size_t n1, const size_t n2) {
            if (n1 == n2) return *this;
            std::swap(m_NIdx[n1], m_NIdx[n2]);
            return *this;
        }

        /// Calculates determinant of this matrix.
        /// @return Determinant of this matrix.
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

        /// Creates a minor from this matrix after removal of given row and column.
        /// @param m0 Row to remove.
        /// @param n0 Column to remove.
        /// @return Minor of this matrix.
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

        /// Transposes this matrix.
        /// @return Transposed matrix.
        [[nodiscard]] mat<N, M> t() const {
            mat<N, M> ret;
            for (size_t m = 0; m < M; ++m) {
                for (size_t n = 0; n < N; ++n) {
                    ret(n, m) = operator()(m, n);
                }
            }
            return ret;
        }

        /// Clamps value in each cell with given values.
        /// @param minVal Minimum allowed value.
        /// @param maxVal Maximum allowed value.
        /// @return Clamped matrix.
        [[nodiscard]] mat clamp(float minVal, float maxVal) const {
            mat ret;
            for (size_t m = 0; m < M; ++m) {
                for (size_t n = 0; n < N; ++n) {
                    ret(m, n) = std::min(std::max(minVal, ret(m, n)), maxVal);
                }
            }
            return ret;
        }

        /// Clamps value in each cell with given values.
        /// @param min Minimum allowed value.
        /// @param max Maximum allowed value.
        /// @return Clamped matrix.
        [[nodiscard]] mat clamp(mat min, mat max) const {
            mat ret;
            for (size_t m = 0; m < M; ++m) {
                for (size_t n = 0; n < N; ++n) {
                    ret(m, n) = std::min(std::max(min(m, n), ret(m, n)), max(m, n));
                }
            }
            return ret;
        }

        /// Calculates a sum of this matrix diagonal.
        /// @return Sum of diagonal.
        [[nodiscard]] float trace() const {
            static_assert(M == N, "Matrix is not square");
            float ret = 0;
            for (size_t m = 0; m < M; ++m) {
                ret += operator()(m, m);
            }
            return ret;
        }

        /// Creates identity matrix.
        /// @return Identity matrix.
        static mat idt() {
            static_assert(M == N, "Matrix is not square");
            mat idtMat;
            for (std::size_t i = 0; i < M; ++i) {
                idtMat(i, i) = 1;
            }
            return idtMat;
        }
    };

    // Global operators

    /// Multiplies matrix by scalar.
    /// @tparam M Number of rows (height).
    /// @tparam N Number of columns (width).
    /// @param f Scalar.
    /// @param m Matrix to multiply.
    /// @return Scaled matrix.
    template <std::size_t M, std::size_t N>
    mat<M, N> operator * (float f, const mat<M, N> &m) {
        return mat<M, N>(m) *= f;
    }

    /// Performs linear interpolation with given bounds.
    /// @tparam M Number of rows (height).
    /// @tparam N Number of columns (width).
    /// @param min Lower bound matrix.
    /// @param max Upper bound matrix.
    /// @param f Factor.
    /// @return Interpolated value.
    template <std::size_t M, std::size_t N>
    mat<M, N> lerp(const mat<M, N> &min, const mat<M, N> &max, float f) {
        return min * (1 - f) + max * f;
    }

}

#endif //LINEAR_MAT_H
