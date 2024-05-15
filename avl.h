#pragma once
#include <utility>
class AVL {
public:
    struct Node {
        Node *l = nullptr, *r = nullptr;
        int sz = 0;
        int h = 0;
        long long val = 0;
        long long y = -1;
        std::pair<int, int> width;
        bool red = false;
        Node(long long x) {
            val = x;
        }
    };

    Node *root = nullptr;


    bool exists(long long x);

    void insert(long long x);
    Node* insert(Node* v, long long x);
    void erase(long long x);
    Node* find(Node* v, long long x);
    int getbal(Node*);

    int geth(Node*);

private:
    void recalc(Node *v);

    Node * balance(Node *v);

    Node *right(Node *x);

    Node * left(Node *x);

    Node *getmax(Node *v);

    Node *getparent(Node *v);

    Node *getparent(Node *v, long long int key);

    Node *getparent(Node *v, Node *fnd);

    Node *erase(Node *v, long long int key);
};