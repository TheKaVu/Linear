#ifndef LINEAR_RATIONAL_H
#define LINEAR_RATIONAL_H

#include <cmath>
#include <numeric>

namespace lnr {
    /// Represents a rational number;
    class rational {
        bool m_Sgn;
        unsigned long m_P;
        unsigned long m_Q;

        void simplify() {
            const auto d = std::gcd(m_P, m_Q);
            m_P /= d;
            m_Q /= d;
        }

    public:

        /// Creates a rational number from a ratio of two given numbers.
        /// @param p Nominator.
        /// @param q Denominator.
        rational(const long p = 0, const long q = 1) {
            if (q == 0) throw std::invalid_argument("q must be different than zero");
            m_P = std::abs(p);
            m_Q = std::abs(q);
            m_Sgn = p < 0 != q < 0;
            simplify();
        }

        /// Creates a rational number by copying other one.
        /// @param other Number to copy.
        rational(const rational& other) = default;

        /// Creates a rational number by moving other one.
        /// @param other Number to move.
        rational(rational&& other) noexcept = default;

        /// Assigns a copy of a rational number to this one.
        /// @param other Number to copy.
        rational& operator=(const rational& other) = default;

        /// Negates a value of this number.
        /// @return Negate rational value.
        rational operator-()  const {
            rational ret = *this;
            ret.m_Sgn = !ret.m_Sgn;
            return ret;
        }

        rational& operator+=(const rational& r){

            auto f = std::lcm(m_Q, r.m_Q);
            m_P *= f / m_Q;
            const auto p = r.m_P * f / r.m_Q;
            if (m_Sgn == r.m_Sgn) {
                m_P += p;
            } else {
                if (m_P >= p) {
                    m_P -= p;
                } else {
                    m_P = p - m_P;
                    m_Sgn = !m_Sgn;
                }
            }
            m_Q = f;
            simplify();
            return *this;
        }

        rational& operator-=(const rational &r) {
            *this += -r;
            return *this;
        }

        rational& operator*=(const rational &r) {
            m_P *= r.m_P;
            m_Q *= r.m_Q;
            m_Sgn = m_Sgn != r.m_Sgn;
            simplify();
            return *this;
        }

        rational& operator/=(const rational &r) {
            if (r.m_P == 0) throw std::invalid_argument("q must be different than zero");
            m_P *= r.m_Q;
            m_Q *= r.m_P;
            m_Sgn = m_Sgn != r.m_Sgn;
            simplify();
            return *this;
        }

        rational operator+(const rational &r) const {
            return rational(*this) += r;
        }

        rational operator-(const rational &r) const {
            return rational(*this) -= r;
        }

        rational operator*(const rational &r) const {
            return rational(*this) *= r;
        }

        rational operator/(const rational &r) const {
            return rational(*this) /= r;
        }

        bool operator==(const rational &r) const {
            return this == &r || (m_P == r.m_P && m_Q == r.m_Q && m_Sgn == r.m_Sgn);
        }

        bool operator!=(const rational &r) const {
            return !(*this == r);
        }

        bool operator<(const rational &r) const {
            const auto prq = m_P * r.m_Q;
            const auto rpq = r.m_P * m_Q;
            if (m_Sgn == r.m_Sgn) return m_Sgn ? prq > rpq : prq < rpq;
            return m_Sgn;
        }

        bool operator<=(const rational &r) const {
            return *this < r || *this == r;
        }

        bool operator>(const rational &r) const {
            return !(*this <= r);
        }

        bool operator>=(const rational &r) const {
            return !(*this < r);
        }

        operator float() const {
            return static_cast<float>(m_P) / static_cast<float>(m_Q) * (m_Sgn ? -1.0f : 1.0f);
        }

        operator double() const {
            return static_cast<double>(m_P) / static_cast<double>(m_Q) * (m_Sgn ? -1.0 : 1.0);
        }
    };
}

#endif //LINEAR_RATIONAL_H
