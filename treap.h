#pragma once
#include <random>
#include <utility>
class Treap {
public:
    struct Node {
        Node* l = nullptr, *r = nullptr;
        long long val = 0, y = 0;
        int sz = 0;
        int h = 0;
        bool red = false;
        std::pair<int, int> width;
        Node(int x) : val(x) {
            std::random_device rd;
            std::mt19937 mt(rd());
            y = mt() % 1000;
        }
        void recalc() {
            sz = 1;
            if (l) sz += l->sz;
            if (r) sz += r->sz;
        }
    };
    Node* root = nullptr;
    bool exists(long long);
    void insert(long long);
    void erase(long long);
private:
    std::pair<Node*, Node*> split(Node*, long long);

    Node* merge(Node*, Node*);

};