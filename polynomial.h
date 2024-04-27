#pragma once

#include "monomial.h"
#include "list.h"
#include <string>
#include <exception>
#include <cmath>
#include <algorithm>
#include "FSM.h"

class Polynomial {
    List<Monomial> lst;
public:

    Polynomial() = default;

    Polynomial(const Polynomial &x) {
        List<Monomial> nw;
        for (auto it = x.lst.begin(); it != x.lst.end(); it = it->next) {
            nw.insert(it->val);
        }
        std::swap(nw, lst);
    }

    Polynomial &operator=(const Polynomial x) {
        List<Monomial> nw;
        for (auto it = x.lst.begin(); it != x.lst.end(); it = it->next) {
            nw.insert(it->val);
        }
        std::swap(nw, lst);
        return *this;
    }

    Polynomial(std::string s) {
        auto res = Convert(s);
        for (auto i: res) {
            lst.insert(i);
        }
    }

    Polynomial(long double x) {
        Monomial m(x);
        lst.insert(m);
    }

    Polynomial(Monomial x) {
        lst.insert(x);
    }


    std::string GetRepresentation() const {
        std::string res;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            if (it->val.IsZero()) {
                continue;
            }
            if (it->val.coef > 0) {
                res += '+';
            }
            std::string cur = std::to_string(it->val.coef);
            if (std::find(cur.begin(), cur.end(), '.') != res.end()) {
                while (!cur.empty() && cur.back() == '0') {
                    cur.pop_back();
                }
            }
            if (cur.back() == '.') {
                cur.pop_back();
            }
            if (cur != "1" || *std::max_element(it->val.pows.begin(), it->val.pows.end()) == 0) {
                res += cur;
            }
            for (int i = 0; i < 26; ++i) {
                if (it->val.pows[i] > 0) {
                    res += 'a' + i;
                    if (it->val.pows[i] > 1) {
                        res += '^';
                        res += std::to_string(it->val.pows[i]);
                    }
                }
            }
        }
        if (!res.empty() && res[0] == '+') {
            res.erase(res.begin());
        } else if (res.empty()) {
            res += '0';
        }

        return res;
    }

    bool IsZero() const {
        bool flag = true;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            if (!it->val.IsZero()) {
                flag = false;
                break;
            }
        }
        return flag;
    }

    Polynomial operator*(const Polynomial b) const {
        Polynomial res;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            for (auto it2 = b.lst.begin(); it2 != b.lst.end(); it2 = it2->next) {
                res.lst.insert(it->val * it2->val);
            }
        }
        return res;
    }

    Polynomial &operator*=(const Polynomial b) {
        return *this = (*this * b);
    }

    Polynomial operator+(const Polynomial b) const {
        Polynomial res(*this);
        for (auto it2 = b.lst.begin(); it2 != b.lst.end(); it2 = it2->next) {
            res.lst.insert(it2->val);
        }
        res = Polynomial(res);
        return res;
    }

    Polynomial &operator+=(const Polynomial b) {
        return *this = (*this + b);
    }

    Polynomial operator-() {
        Polynomial res(*this);
        for (auto it = res.lst.begin(); it != res.lst.end(); it = it->next) {
            it->val.coef *= -1;
        }
        return res;
    }

    Polynomial operator-(Polynomial b) {
        Polynomial res(*this + (-b));
        return res;
    }

    Polynomial &operator-=(const Polynomial b) {
        return *this = Polynomial(*this - b);
    }

    Polynomial GetDeriative(char c) {
        if (!IsPresent(c)) {
            throw std::runtime_error("char not present polynomial");
        }
        int num = c - 'a';
        Polynomial res;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            if (it->val.pows[num] == 0) {
                continue;
            }
            Monomial cp = it->val;
            cp.coef *= cp.pows[num];
            cp.pows[num]--;
            res.lst.insert(cp);
        }
        return res;

    }

    Polynomial GetXDeriative(char c, int x) const {
        Polynomial cur = *this;
        for (int i = 0; i < x; ++i) {
            cur = cur.GetDeriative(c);
        }
        return cur;
    }


    long double GetValue(std::vector<long double> &v) const {
        long double res = 0;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            long double c = it->val.coef;
            for (int j = 0; j < 26; ++j) {
                c *= pow(v[j], it->val.pows[j]);
                if (c > INFINITYMY) {
                    throw std::runtime_error("numbers too big");
                }
            }
            res += c;
            if (res > INFINITYMY) {
                throw std::runtime_error("numbers too big");
            }
        }
        return res;
    }

    bool IsSingleLetter() const {
        std::vector<long long> maxpow(26);
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            for (int j = 0; j < 26; ++j) {
                maxpow[j] = std::max(maxpow[j], it->val.pows[j]);
            }
        }
        std::nth_element(maxpow.begin(), maxpow.begin() + 24, maxpow.end());
        if (*std::max_element(maxpow.begin(), maxpow.end()) == 0) {
            return false;
        }
        return maxpow[24] == 0;
    }

    long double GetFree() const {
        long double res = 0;
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            if (*std::max_element(it->val.pows.begin(), it->val.pows.end()) == 0) res += it->val.coef;
        }
        return res;
    }

    char GetSingleChar() const {
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            for (int j = 0; j < 26; ++j) {
                if (it->val.pows[j] > 0) {
                    return 'a' + j;
                }
            }
        }
        return '#';
    }

    bool IsPresent(char c) {
        for (auto it = lst.begin(); it != lst.end(); it = it->next) {
            if (it->val.pows[c - 'a'] > 0) return true;
        }
        return false;
    }

    std::vector<long double> GetRoots() const {
        if (!IsSingleLetter()) {
            throw std::runtime_error("trying to take roots from polynomial with nome or multuple letters");
        }
        std::vector<long double> res;
        long long x = GetFree();
        if (x == 0) {
            return {0};
        }
        std::vector<long double> vec(26);
        int ind = GetSingleChar() - 'a';
        for (int i = 1; i * i <= x; ++i) {
            if (x % i == 0) {
                vec[ind] = i;
                if (std::abs(GetValue(vec)) < 1e-7) res.push_back(i);
                vec[ind] = -i;
                if (std::abs(GetValue(vec)) < 1e-7) res.push_back(-i);
                if (x / i != i) {
                    vec[ind] = x / i;
                    if (std::abs(GetValue(vec)) < 1e-7) res.push_back(x / i);
                    vec[ind] = -(x / i);
                    if (std::abs(GetValue(vec)) < 1e-7) res.push_back(-(x / i));
                }
            }
        }
        return res;

    }

    Polynomial operator/(const Polynomial b) const {
        if (b.IsZero()) {
            throw std::runtime_error("division by zero");
        }
        if (b.GetSingleChar() == '#') {
            Polynomial cur(*this);
            for (auto it = cur.lst.begin(); it != cur.lst.end(); it = it->next) {
                it->val = it->val / b.lst.begin()->val;
            }
            return cur;
        }
        if (!IsSingleLetter() || !b.IsSingleLetter()) {
            throw std::runtime_error("divison with polynomials with multiple letters");
        }
        Polynomial res;
        Polynomial cur(*this);
        while (cur.lst.size() > 0) {
            if (*std::max_element(cur.lst.begin()->val.pows.begin(), cur.lst.begin()->val.pows.end()) <
                *std::max_element(b.lst.begin()->val.pows.begin(), b.lst.begin()->val.pows.end())) {
                break;
            }
            Monomial x = cur.lst.begin()->val / b.lst.begin()->val;
            Polynomial cc = (Polynomial) x * b;
            cur = cur - cc;
            res += x;
        }
        return res;
    }

    Polynomial &operator/=(const Polynomial b) {
        return *this = *this / b;
    }

    Polynomial operator%(Polynomial b) {
        return *this - b * (*this / b);
    }

    bool operator==(Polynomial b) {
        if (lst.size() != b.lst.size()) return false;
        auto it1 = lst.begin(), it2 = b.lst.begin();
        for (; it2 != b.lst.end(); it2=it2->next, it1=it1->next) {
            if (it1->val.pows != it2->val.pows || std::abs(it1->val.coef - it2->val.coef) > 1e-7) {
                return false;
            }
        }
        return true;
    }

    bool operator>(Polynomial b) {
        if (lst.size() != b.lst.size()) return lst.size() < b.lst.size();
        auto it1 = lst.begin(), it2 = b.lst.begin();
        for (; it2 != b.lst.end(); it2=it2->next, it1=it1->next) {
            if (it1->val.pows != it2->val.pows || std::abs(it1->val.coef - it2->val.coef) > 1e-7) {
                return (it1->val.pows < it2->val.pows ||
                        (it1->val.pows == it2->val.pows && it1->val.coef - it2->val.coef < -1e-7));
            }
        }
        return false;
    }



    bool operator<(Polynomial b) {
        return b > *this;
    }

};