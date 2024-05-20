#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
int CURRAD = 30;
template<typename T>
struct Circle {
    T *node;
    long double x = 0, y = 0;
    int radius = CURRAD;
};


std::map<int, int> cntw;

template<typename T>
void calcsize(T *v, int h) {
    v->sz = 1;
    cntw[h]++;
    v->width = {1, 1};
    if (v->l) {
        calcsize(v->l, h + 1);
        v->sz += v->l->sz;
        v->width.first = v->l->width.first + 1;
    }
    if (v->r) {
        calcsize(v->r, h + 1);
        v->sz += v->r->sz;
        v->width.second = v->r->width.second + 1;
    }
}

std::map<int, int> curcnt;

template<typename T>
int getwidth(T *v) {
    if (v == nullptr) {
        return 1;
    }
    return v->sz;
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> segs;

void drawsegment(sf::Vector2f a, sf::Vector2f b) {
    //a.y += 10, b.y += 10;
    //a.x += 5, b.x += 5;
    a.x += CURRAD, a.y += CURRAD;b.x += CURRAD, b.y += CURRAD;
    segs.push_back({a, b});
}
long long curc = 1020;
template<typename T>
std::vector<Circle<T>> arrange_tree(T *v, int h, long double addx, long double curw) {
    std::vector<Circle<T>> result;
    curcnt[h]++;
    long double wl = 0, wr = 0;
    if (!v->l && !v->r) {
        result.push_back({v, curw / 2 + addx, ((long double)curc / cntw.rbegin()->first) * h});
    } else {
        wl = (curw - 2 * CURRAD) * ((long double) getwidth(v->l) / (getwidth(v->l) + getwidth(v->r)));
        wr = (curw - 2 * CURRAD) * ((long double) getwidth(v->r) / (getwidth(v->l) + getwidth(v->r)));
        result.push_back({v, wl + CURRAD + addx, ((long double)curc / cntw.rbegin()->first) * h});
    }
    if (v->l) {
        auto r1 = arrange_tree(v->l, h + 1, addx, wl);
        drawsegment({result[0].x, result[0].y}, {r1[0].x, r1[0].y});
        for (auto i: r1) result.push_back(i);
    }
    if (v->r) {
        auto r1 = arrange_tree(v->r, h + 1, addx + wl + 2 * CURRAD, wr);
        drawsegment({result[0].x, result[0].y}, {r1[0].x, r1[0].y});
        for (auto i: r1) result.push_back(i);
    }
    return result;
}

template<typename T>
void drawtree(T tree, sf::RenderWindow &w, bool rendervalues, bool renderpriority, long double curcoef) {
    if (!tree.root) {
        return;
    }
    cntw = {};
    calcsize(tree.root, 0);
    curc = 200 * cntw.rbegin()->first;
    segs.clear();
    long long prefcoef = curcoef;
    //curcoef += tree.root->sz / 7.5;
    //if (tree.root->sz > 50) curcoef *= 1.5;
    //if (tree.root->sz > 150) curcoef *= 1.25;
    //curc =
    auto pnt = arrange_tree(tree.root, 0, 0, 1800 + curcoef * tree.root->sz);
    sf::CircleShape basic(CURRAD);
    basic.setOutlineColor(sf::Color(255, 255, 255));
    basic.setOutlineThickness(2.5);
    basic.setFillColor(sf::Color(0, 0, 0));
    basic.setPointCount(1000);
    for (auto i: segs) {
        sf::Vertex line[] =
                {
                        sf::Vertex(i.first),
                        sf::Vertex(i.second)
                };
        w.draw(line, 2, sf::Lines);
    }
    if (pnt.size() == 1) {
        pnt[0].x = 900;
        pnt[0].y = 540;
    }
    for (auto i: pnt) {
        basic.setRadius(i.radius);
        basic.setPosition(i.x, i.y);
        if (i.node->red) {
            basic.setOutlineColor(sf::Color::Red);
        } else {
            basic.setOutlineColor(sf::Color(255, 255, 255));
        }
        w.draw(basic);
    }
    sf::Font fn;
    fn.loadFromFile("/home/kosmarik/CLionProjects/game_of_life/write.ttf");
    for (auto i : pnt) {
        std::string s = "";
        if (rendervalues) {
            s += std::to_string(i.node->val);
        }
        if (i.node->y != -1 && renderpriority) {
            if (!s.empty()) s += '\n';
            s += std::to_string(i.node->y);
        }
        sf::Text text;
        text.setString(s);
        text.setFont(fn);
        text.setCharacterSize(CURRAD * 1.3 * 1.7 / std::max((int)s.size(), 2));
        if (i.node->y != -1 && renderpriority && rendervalues) {
            text.setCharacterSize(text.getCharacterSize() * 1.9);
        }
        text.setFillColor(sf::Color::White);
        auto rs = text.getGlobalBounds();
        text.setPosition(i.x - (long double)rs.width / 2 + CURRAD, i.y - (long double)rs.height / 2 + CURRAD / 1.25);
        w.draw(text);
    }
    curcoef = prefcoef;
}
