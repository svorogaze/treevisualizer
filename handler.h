#pragma once
#include "SFML/Graphics.hpp"
#include "splay.h"
#include "treap.h"
#include "textbox.h"
#include "rbtree.h"
#include "avl.h"
#include <set>
class Handler {
public:
    int curt = 3;
    Treap treap;
    Splay splay;
    AVL avl;
    RBtree rbtree;
    sf::RenderWindow w;
    std::mt19937 we;
    TextBox command;
    sf::View curview;
    long double curcoef = 2;
    bool changed = false;
    bool rendertext = false;
    bool renderpriority = false;
    bool rendervalues = true;
    long long curg = 1;
    std::set<long long> st;
    Handler();
    void HandleEvent();
    void DrawTree();
    void OnResize();

};
