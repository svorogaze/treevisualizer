#pragma once
#include "treap.h"
#define Node Treap::Node
using std::pair;

pair<Node*, Node*> Treap::split(Node* t, long long key) {
    if (t == nullptr) return {nullptr, nullptr};
    if (t->val > key) {
        auto res = split(t->l, key);
        t->l = res.second;
        t->recalc();
        return {res.first, t};
    } else {
        auto res = split(t->r, key);
        t->r = res.first;
        t->recalc();
        return {t, res.second};
    }
}

Node* Treap::merge(Node* t1, Node* t2) {
    if (t1 == nullptr) return t2;
    if (t2 == nullptr) return t1;
    if (t2->y > t1->y) {
        t2->l = merge(t1, t2->l);
        if(t2->l) t2->l->recalc();
        t2->recalc();
        return t2;
    } else {
        t1->r = merge(t1->r, t2);
        if (t1->r) t1->r->recalc();
        t1->recalc();
        return t1;
    }
}


long long get_max(Node* r) {
    if (!r) return -2e9;
    while (r->r) {
        r = r->r;
    }
    return r->val;
}

bool Treap::exists(long long x) {
    auto [t1, t2] = split(root, x);
    bool res = (get_max(t1) == x);
    root = merge(t1, t2);
    return res;
}

void Treap::insert(long long x) {
    if (!exists(x)) {
        Node* v = new Node(x);
        auto[t1, t2] = split(root, x);
        root = merge(t1, merge(v, t2));
    }
}

void Treap::erase(long long x) {
    if (exists(x)) {
        auto[t1, t2] = split(root, x);
        auto[t3, t4] = split(t1, x - 1);
        root = merge(t3, t2);
    }
}