#pragma once
#include <utility>
class Splay {
public:
    struct Node {
        Node *l = nullptr, *r = nullptr;
        int sz = 0;
        long long val = 0;
        std::pair<int, int> width;
        bool red = false;
        long long y = -1;
        Node(long long x) {
            val = x;
        }
    };

    Node *root = nullptr;

    Node *right(Node *x);

    Node *left(Node *x);

    Node *splay(Node *v, long long key);

    bool exists(long long x);

    void chain(Node* v, long long key);

    void insert(long long x);

    long long get_min(Node* v);

    Node* merge(Node* l, Node* r);

    void erase(long long x);

    void recalcsize(Node* v);

private:
};