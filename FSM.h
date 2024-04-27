#include <string>
#include "monomial.h"

std::vector<Monomial> Convert(std::string s) {
    std::vector<Monomial> res;
    int state = 0, sign = 1;
    Monomial cur(1);
    int ind = 0;
    std::string str;
    char last = '-';
    for (int i = 0; i + 1 < s.size(); ++i) {
        if ((s[i] == '-' || s[i] == '+') && (s[i + 1] == '-' || s[i + 1] == '+')) {
            throw std::runtime_error("string is not in proper format");
        }
    }
    if (s.back() != '+' && s.back() != '-') {
        s += '+';
    } else {
        throw std::runtime_error("string is not in proper format");
    }
    for (; ind < s.size();) {
        char c = s[ind];
        if (state == 0) {
            last = '-';
            if (c == '-') {
                sign = -1;
                state = 1;
            } else if (c == '+') {
                sign = 1;
                state = 1;
            } else if (c >= '0' && c <= '9') {
                state = 1;
                ind--;
            } else if (c >= 'a' && c <= 'z') {
                state = 1;
                ind--;
            } else {
                throw std::runtime_error("string is not in proper format");
            }
        } else if (state == 1) {
            if (c >= 'a' && c <= 'z') {
                cur.pows[c - 'a']++;
                if (!str.empty()) cur.coef *= std::stold(str);
                last = c;
                str = "";
                state = 1;
            } else if ((c >= '0' && c <= '9') || c == '.') {
                if (c == '.' && str.empty()) {
                    throw std::runtime_error("string is not in proper format");
                }
                str += c;
                state = 1;
                last = '#';
            } else if (c == '^') {
                if (!str.empty() || last == '#') {
                    throw std::runtime_error("string is not in proper format");
                }
                state = 2;
            } else if (c == '+' || c == '-') {
                if (!str.empty()) cur.coef *= std::stold(str);
                cur.coef *= sign;
                res.push_back(cur);
                cur = Monomial(1);
                str = "";
                ind--;
                state = 0;
            } else {
                throw std::runtime_error("string is not in proper format");
            }
        } else if (state == 2) {
            if ((c >= 'a' && c <= 'z') || c == '+' || c == '-') {
                if (str.empty()) {
                    throw std::runtime_error("string is not in proper format");
                }
                cur.pows[last - 'a'] += std::stoll(str) - 1;
                str = "";
                state = 1;
                ind--;
            } else if (c >= '0' && c <= '9') {
                str += c;
            } else {
                throw std::runtime_error("string is not in proper format");
            }
        }
        ind++;
    }
    return res;
}

