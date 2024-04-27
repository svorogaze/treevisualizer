#pragma once
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cmath>
constexpr long double INFINITYMY = 1e18;
class Monomial {

public:
    long double coef = 0;
    std::vector<long long> pows;
    Monomial() = default;

    template<typename T>
    Monomial(T a, std::vector<T> b) {
        if (!std::is_convertible_v<T, long long>) {
            throw std::logic_error("constructing monomial from unconvertable type");
        }
        if (b.size() != 26) {
            throw std::logic_error("constructing monomial from unappropriate vector");
        }
        pows.resize(26);
        coef = a;
        for (int i = 0; i < 26; ++i) {
            pows[i] = b[i];
        }
    }
    template<typename T>
    Monomial(T x) {
        coef = x;
        pows.resize(26);
    }

    Monomial(Monomial const &x) {
        pows.resize(26);
        coef = x.coef;
        for (int i = 0; i < 26; ++i) {
            pows[i] = x.pows[i];
        }
    }

    Monomial operator*(const Monomial b) const {
        Monomial res(*this);
        res.coef *= b.coef;
        if (std::abs(res.coef) > INFINITYMY) {
            throw std::overflow_error("coefficent numbers too big");
        }
        for (int i = 0; i < 26; ++i) {
            res.pows[i] += b.pows[i];
            if (std::abs(res.pows[i]) > INFINITYMY) {
                throw std::overflow_error("powers numbers too big");
            }
        }
        return res;
    }

    Monomial &operator*=(Monomial b) {
        return *this = *this * b;
    }

    Monomial operator+(const Monomial b) const {
        Monomial res(*this);
        if (pows != b.pows) {
            throw std::logic_error("powers don't match in addition");
        }
        res.coef += b.coef;
        if (std::abs(res.coef) > INFINITYMY) {
            throw std::overflow_error("coefficent numbers too big");
        }
        return res;
    }

    Monomial &operator+=(const Monomial b) {
        return *this = *this + b;
    }

    Monomial operator-() const {
        Monomial cur(*this);
        cur.coef = -coef;
        return cur;
    }

    Monomial operator-(Monomial b) const {
        return (*this + (-b));
    }

    Monomial operator-=(Monomial b) {
        return *this = (*this + (-b));
    }

    Monomial operator/(const Monomial b) const {
        if (b.IsZero()) {
            throw std::runtime_error("divison by zero");
        }
        Monomial res(*this);
        res.coef /= b.coef;
        for (int i = 0; i < 26; ++i) {
            res.pows[i] -= b.pows[i];
            if (res.pows[i] < 0) {
                throw std::logic_error("powers become not natural");
            }
        }
        return res;
    }

    bool operator>(const Monomial b) const {
        return pows > b.pows;
    }

    bool operator==(const Monomial b) const {
        return pows == b.pows;
    }

    bool operator<(const Monomial b) const {
        return b > *this;
    }

    bool operator>=(const Monomial b) const {
        return *this > b || *this == b;
    }

    bool operator <=(const Monomial b) const  {
        return *this < b || *this == b;
    }

    bool IsZero() const {
        return std::abs(coef) <= 1e-9;
    }




};

