#ifndef LINEAR_VEC_H
#define LINEAR_VEC_H
#include <cmath>
#include <algorithm>
#include "mat.h"

namespace lnr {
    /// A subtype of N-sized single-column matrix.
    /// @tparam N Vector size.
    template <std::size_t N>
    class vec : public mat<N, 1> {

    public:
        /// Creates new vector filled with given value.
        /// @param value Value to fill every cell with.
        vec(float value = 0.f) : mat<N, 1>(value) {}

        /// Creates new vector from a static array.
        /// @param values Input array.
        vec(const float (&values)[N]) : mat<N, 1>(values) {}

        /// Creates new vector with given values in cells.
        /// @param values Values to put in cells.
        template<typename ...Args>
        constexpr vec(Args... values) : mat<N, 1>(values...) {}

        /// Creates new vector by copying other one.
        /// @param other Vector to copy.
        vec(const vec &other) : mat<N, 1>(other) {}

        /// Creates new vector by moving other one.
        /// @param other Vector to move.
        vec(vec &&other) noexcept : mat<N, 1>(std::move(other)) {}

        /// Destroys this vector object.
        ~vec() override = default;

        /// Gets the value of vector cell.
        /// @param i Cell index.
        /// @return Value of respective cell.
        float operator[](const std::size_t i) const {
            return mat<N, 1>::operator()(i, 0);
        }

        /// Accesses the value of vector cell.
        /// @param i Cell index.
        /// @return Reference to value of respective cell.
        float& operator[](const std::size_t i) {
            return mat<N, 1>::operator()(i, 0);
        }

        /// Calculates squared magnitude of this vector.
        /// @return Squared magnitude of this vector.
        [[nodiscard]] float mag2() const {
            float sum_s = 0;
            for (std::size_t i = 0; i < N; ++i) {
                sum_s += operator[](i) * operator[](i);
            }
            return sum_s;
        }

        /// Calculates magnitude of this vector.
        /// @return Magnitude of this vector.
        [[nodiscard]] float mag() const {
            return sqrtf(mag2());
        }

        /// Calculates normalized form of this vector.
        /// @return Normalized vector.
        [[nodiscard]] vec norm() const {
            return *this / mag();
        }

        /// Creates versor of a given axis.
        /// @param i Index of the axis.
        /// @return Versor of given axis.
        static vec versor(const std::size_t i) {
            vec ret;
            ret[i] = 1;
            return ret;
        }
    };

    /// Calculates angle between two given vectors
    /// @tparam N Vector size.
    /// @param v1 First vector.
    /// @param v2 Second vector.
    /// @return Angle in radians between two vectors.
    template <std::size_t N>
    float angle(const vec<N> &v1, const vec<N> &v2) {
        return acosf((v1.t() * v2)(0, 0) / (v1.mag() * v2.mag()));
    }

    /// Calculates dot product of two given vectors.
    /// @tparam N Vector size.
    /// @param v1 First vector.
    /// @param v2 Second vector.
    /// @return Dot product of two vectors.
    template <std::size_t N>
    float dot(const vec<N> &v1, const vec<N> &v2) {
        return (v1.t() * v2)(0, 0);
    }

    /// Calculates cross product of two given vectors.
    /// @param v1 First vector.
    /// @param v2 Second vector.
    /// @return Cross product of two vectors.
    inline vec<3> cross(const vec<3> &v1, const vec<3> &v2) {
        return {
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
        };
    }
}

#endif //LINEAR_VEC_H