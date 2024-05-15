#include "rbtree.h"
#include <algorithm>
#define Node RBtree::Node

Node* RBtree::find(Node* v, long long key) {
    if (!v) return nullptr;
    if (v->val == key) {
        return v;
    }
    if (v->val > key) {
        return find(v->l, key);
    } else {
        return find(v->r, key);
    }
}

bool RBtree::exists(long long x) {
    return find(root, x) != nullptr;
}

bool RBtree::getcolor(Node* v) { // red - true, black - false
    if (!v) return false;
    return v->red;
}

Node* RBtree::left(Node *h) {
    Node* x = h->r;
    h->r = x->l;
    x->l = h;
    x->red = h->red;
    h->red = true;
    return x;
}

Node* RBtree::right(Node *h) {
    Node* x = h->l;
    h->l = x->r;
    x->r = h;
    x->red = h->red;
    h->red = true;
    return x;
}

void flip(Node* v) {
    v->red ^= 1;
    if (v->l) v->l->red ^= 1;
    if (v->r) v->r->red ^= 1;
}

Node* RBtree::fixup(Node* v) {
    if (getcolor(v->r) && !getcolor(v->l)) {
        v = left(v);
    }
    if (getcolor(v->l) && getcolor(v->l->l)) {
        v = right(v);
    }
    if (getcolor(v->l) && getcolor(v->r)) flip(v);
    return v;
}

Node* RBtree::insert(Node* v, long long key) {
    if (v == nullptr) {
        v = new Node(key);
        v->red = true;
        return v;
    }
    if (v->val > key) {
        v->l = insert(v->l, key);
    } else {
        v->r = insert(v->r, key);
    }
    return fixup(v);
}

void RBtree::insert(long long x) {
    if (exists(x)) return;
    root = insert(root, x);
    root->red = false;
}

Node* RBtree::deletemin(Node* v) {
    if (!v->l) return nullptr;
    if (!getcolor(v->l) && !getcolor(v->l->l)) {
        v = redleft(v);
    }
    v->l = deletemin(v->l);
    return fixup(v);
}

Node* RBtree::redleft(Node* v) {
    flip(v);
    if (getcolor(v->r->l)) {
        v->r = right(v->r);
        v = left(v);
        flip(v);
    }
    return v;
}

Node* RBtree::redright(Node* v) {
    flip(v);
    if (getcolor(v->l->l)) {
        v = right(v);
        flip(v);
    }
    return v;
}

Node* min(Node* v) {
    if (!v->l) return v;
    return min(v->l);
}

Node* RBtree::erase(Node* v, long long val) {
    if (val < v->val) {
        if (!getcolor(v->l) && !getcolor(v->l->l)) {
            v = redleft(v);
        }
        v->l = erase(v->l, val);
    } else {
        if (getcolor(v->l)) {
            v = right(v);
        }
        if (val == v->val && !v->r) {
            return nullptr;
        }
        if (!getcolor(v->r) && !getcolor(v->r->l)) {
            v = redright(v);
        }
        if (v->val == val) {
            v->val=min(v->r)->val;
            v->r = deletemin(v->r);
        } else {
            v->r = erase(v->r, val);
        }
    }
    return fixup(v);
}

void RBtree::erase(long long x) {
    if (!exists(x)) return;
    root = erase(root, x);
}
