#pragma once
#include "splay.h"
#define Node Splay::Node

Node* Splay::right(Node *x) {
    Node *y = x->l;
    x->l = y->r;
    y->r = x;
    return y;
}

Node * Splay::left(Node *x) {
    Node *y = x->r;
    x->r = y->l;
    y->l = x;
    return y;
}

Node *Splay::splay(Node *v, long long key) {
    if (!v) {
        return nullptr;
    }
    if (v->val == key) {
        return v;
    }
    if (v->val > key) {
        if (v->l && v->l->val > key && v->l->l) { //zig-zig case
            auto rs = splay(v->l->l, key);
            if (rs == nullptr) return nullptr;
            v->l->l = rs;
            v = right(v);
            return right(v);
        } else if (v->l) { //the rest
            auto vl = splay(v->l, key);
            if (vl == nullptr) return nullptr;
            v->l = vl;
            return right(v);
        }
    } else {
        if (v->r && v->r->val < key && v->r->r) { //zag-zag case
            auto rs = splay(v->r->r, key);
            v->r->r = rs;
            v = left(v);
            return left(v);
        } else if (v->r) { //the rest
            auto vr = splay(v->r, key);
            if (!vr) return nullptr;
            v->r = vr;
            return left(v);
        }
    }
    return v;
}

bool Splay::exists(long long x) {
    if (!root) return false;
    auto rs = splay(root, x);
    if (rs == nullptr) return false;
    root = rs;
    return root->val == x;
}

void Splay::chain(Node *v, long long key) {
    if (v->val > key) {
        if (!v->l) {
            v->l = new Node(key);
        } else {
            chain(v->l, key);
        }
    } else {
        if (!v->r) {
            v->r = new Node(key);
        } else {
            chain(v->r, key);
        }
    }
}

void Splay::insert(long long x) {
    if (exists(x)) return;
    if (root == nullptr) {
        root = new Node(x);
    } else {
        chain(root, x);
        //root = splay(root, x);
    }
}

long long Splay::get_min(Node *v) {
    if (!v->l) return v->val;
    return get_min(v->l);
}

Node *Splay::merge(Node *l, Node *r) {
    if (!r) return l;
    if (!l) return r;
    r = splay(r, get_min(r));
    r->l = l;
    return r;
}

void Splay::erase(long long x) {
    if (!exists(x)) return;
    root = splay(root, x);
    root = merge(root->l, root->r);
}

void Splay::recalcsize(Node *v) {
    v->sz = 1;
    if (v->l) {
        recalcsize(v->l);
        v->sz += v->l->sz;
    }
    if (v->r) {
        recalcsize(v->r);
        v->sz += v->r->sz;
    }
}
