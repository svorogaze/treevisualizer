#pragma once
#include <algorithm>
#include <stdexcept>
template<typename T>
class List {
    class Node {
    public:
        T val;
        Node *prev = nullptr, *next = nullptr;

        Node() = default;

        Node(T x) {
            val = x;
        }

        Node* operator++() const {
            return next;
        }

        Node* operator--() const {
            return prev;
        }


    };
public:
    bool plusequal = true;
    bool ignorezero = true;
    List() = default;

    List& operator=(const List& x) {
        List nw;
        int i = 0;
        for (auto it = x.begin(); it != x.end(); it=it->next) {
            nw.insert(it->val);
        }
        std::swap(root, nw.root);
        std::swap(sz, nw.sz);
        return *this;
    }

    List(const List& x) {
        List nw;
        for (auto it = x.begin(); it != x.end(); it=it->next) {
            nw.insert(it->val);
        }
        std::swap(root, nw.root);
        std::swap(sz, nw.sz);
    }

    int size() const {
        return sz;
    }

    Node * begin() const {
        return root;
    }

    Node * end() const {
        return nullptr;
    }



    void insert(T x) {
        if (x.IsZero() && ignorezero) {
            return;
        }
        Node* c = root;
        Node* prev = root;
        while (c != nullptr && x < c->val) {
            prev = c;
            c = c->next;
        }
        if (plusequal && c != nullptr && x == c->val) { //found equal
            c->val += x;
            return;
        }
        if (c == nullptr) {
            if (prev == nullptr) { //empty list
                if (sz != 0) {
                    throw std::logic_error("trying to insert in non-empty list as if it was an empty one");
                }
                root = new Node;
                root->val = x;
                root->next = nullptr;
            } else { // insert to end
                Node* v = new Node;
                v->val = x;
                prev->next = v;
                v->prev = prev;
                v->next = nullptr;
            }
            sz++;
            return;
        }
        if (c == prev) { // insert to begin
            Node* v = new Node;
            v->val = x;
            root = v;
            c->prev = v;
            v->next = c;
            v->prev = nullptr;
            sz++;
            return;
        }
        Node* v = new Node;
        v->val = x;
        prev->next = v;
        v->prev = prev;
        v->next = c;
        c->prev = v;
        sz++;
    }

    ~List() {
        Node *cur = root;
        for (int i = 0; i < sz; ++i) {
            auto next = cur->next;
            delete cur;
            cur = next;
        }

    }

    void erase(int ind) {
        if (ind >= sz || ind < 0) {
            throw std::runtime_error("out of bounds index");
        }
        sz--;
        if (ind == 0) {
            root = root->next;
            return;
        }
        auto it = root;
        for (int i = 0; i < ind; it = it->next, i++) {
        }
        it->prev->next = it->next;
        if (ind + 1 < sz) {
            it->next->prev = it->prev;
        }
    }

    Node operator[](int i) {
        if (i >= sz || i < 0) {
            throw std::runtime_error("out of bounds index");
        }
        auto it = root;
        while (i--) {
            it = it->next;
        }
        return *it;
    }

private:
    Node* root = nullptr;
    int sz = 0;


};