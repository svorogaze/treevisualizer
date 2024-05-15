#include <algorithm>
#include "avl.h"
#define Node AVL::Node

int AVL::geth(Node* v) {
    if (!v) return 0;
    return v->h;
}

int AVL::getbal(Node* v) {
    if (!v) return 0;
    return geth(v->l) - geth(v->r);
}

void AVL::recalc(Node* v) {
    if (!v) return;
    v->h = std::max(geth(v->l), geth(v->r)) + 1;
}

Node* AVL::right(Node *x) {
    Node *y = x->l;
    x->l = y->r;
    y->r = x;
    recalc(x->l);
    recalc(x->r);
    recalc(y->l);
    recalc(y->r);
    recalc(x);
    recalc(y);
    return y;
}

Node* AVL::left(Node *x) {
    Node *y = x->r;
    x->r = y->l;
    y->l = x;
    recalc(x->l);
    recalc(x->r);
    recalc(y->l);
    recalc(y->r);
    recalc(x);
    recalc(y);
    return y;
}

Node* AVL::balance(Node* v) {
    if (!v) return nullptr;
    recalc(v->l);
    recalc(v->r);
    recalc(v);
    if (getbal(v) > 1) {
        if (getbal(v->l) >= 0) {
            return right(v);
        } else {
            v->l = left(v->l);
            return right(v);
        }
    }
    if (getbal(v) < -1) {
        if (getbal(v->r) <= 0) {
            return left(v);
        } else {
            v->r = right(v->r);
            return left(v);
        }
    }
    return v;
}

Node* AVL::insert(Node* v, long long key) {
    if (v == nullptr) {
        v = new Node(key);
        return v;
    }
    if (v->val == key) return v;
    if (v->val > key) {
        v->l = insert(v->l, key);
    } else if (v->val < key) {
        v->r = insert(v->r, key);
    }

    return balance(v);
}


void AVL::insert(long long key) {
    root = insert(root, key);
}

Node* AVL::find(Node* v, long long key) {
    if (v == nullptr) {
        return nullptr;
    }
    if (v->val == key) return v;
    if (v->val < key) {
        return find(v->r, key);
    } else {
        return find(v->l, key);
    }
    return nullptr;
}

Node* AVL::getmax(Node* v) {
    if (!v->r) return v;
    return getmax(v->r);
}

Node* AVL::getparent(Node* v, Node* fnd) {
    if (v->l == fnd || v->r == fnd) {
        return v;
    }
    if (v->val > fnd->val) {
        return getparent(v->l, fnd);
    } else {
        return getparent(v->r, fnd);
    }
}

Node* AVL::erase(Node* v, long long key) {
    if (v->val > key) {
        v->l = erase(v->l, key);
    } else if (v->val < key) {
        v->r = erase(v->r, key);
    } else {
        if (!(v->l) || !(v->r)) {
            v = (v->l ? v->l : v->r);
        } else {
            Node* re = getmax(v->l);
            v->val = re->val;
            v->l = erase(v->l, re->val);
        }
        return balance(v);
    }
    return balance(v);
}

void AVL::erase(long long key) {
    if (!exists(key)) return;
    erase(root, key);
}

bool AVL::exists(long long key) {
    Node* res = find(root, key);
    return res != nullptr;
}


