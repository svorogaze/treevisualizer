#pragma once
#include "monomial.h"
#include "list.h"
#include "polynomial.h"
#include <iostream>
#include "Handler.h"
signed main() {
    Handler h;
    while (true) {
        h.HandleEvent();
        h.PrintPolynomials();
    }
}