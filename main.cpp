#pragma once
#include "handler.h"

signed main() {
    Handler h;
    while (true) {
        h.HandleEvent();
        h.DrawTree();
    }
}