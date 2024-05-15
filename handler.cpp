#pragma once

#include "SFML/Graphics.hpp"
#include "splay.h"
#include "treap.h"
#include "visualizer.h"
#include "avl.h"
#include "handler.h"
#include <iostream>
#include <sstream>
#include <exception>

Handler::Handler() : w(sf::VideoMode(1920, 1080), "tree visualizer") {
}


void Handler::HandleEvent() {
    sf::Event event{};
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
            exit(0);
        } else if (event.type == sf::Event::Resized) {
            OnResize();
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            rendertext ^= 1;
        } else if (rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            std::stringstream ss(command.str);
            std::string a, b, c;
            if (ss >> a) {
                if (a == "insert") {
                    try {
                        long long x;
                        ss >> x;
                        treap.insert(x);
                        splay.insert(x);
                        avl.insert(x);
                        rbtree.insert(x);
                        st.insert(x);
                    } catch (std::exception &e) {

                    }
                } else if (a == "rand") {
                    try {
                        long long x;
                        ss >> x;
                        std::random_device rd;
                        std::mt19937 mt(rd());
                        long long opern = 0;
                        long long succ = 0;
                        for (int i = 0;; ++i) {
                            if (succ == x) break;
                            long long xe = mt() % curg;
                            opern++;
                            if (opern == 1e6) {
                                curg *= 10;
                                continue;
                            }
                            if (st.find(xe) != st.end()) {
                                continue;
                            }
                            st.insert(xe);
                            treap.insert(xe);
                            succ++;
                            opern = 0;
                            splay.insert(xe);
                            avl.insert(xe);
                            rbtree.insert(xe);
                        }
                    } catch (std::exception &e) {

                    }
                }
            }
            command.str = "";
            rendertext = false;
        } else if (rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            command.str += " ";
        }
        if (rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace) {
            if (!command.str.empty()) {
                command.str.pop_back();
            }
        } else if (rendertext && event.type == sf::Event::TextEntered) {
            if (event.text.unicode > 32 && event.text.unicode < 128) {
                command.str += static_cast<char>(event.text.unicode);
            }
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
            curt = 0;
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
            curt = 1;
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) {
            curt = 2;
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num4) {
            curt = 3;
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num5) {
            rendervalues ^= 1;
        } else if (!rendertext && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num6) {
            renderpriority ^= 1;
        } else if (!rendertext && event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Right) {
            long long val = -1337;
            int x = event.mouseButton.x, y = event.mouseButton.y;
            std::cout << x << ' ' << y << '\n';
            if (curt == 0) {
                auto pn = arrange_tree(treap.root, 0, 0, 1800);
                for (auto i: pn) {
                    if ((x - (i.x + CURRAD)) * (x - (i.x + CURRAD)) + (y - (i.y + CURRAD)) * (y - (i.y + CURRAD)) <=
                        CURRAD * CURRAD) {
                        val = i.node->val;
                        break;
                    }
                }
            } else if (curt == 1) {
                auto pn = arrange_tree(splay.root, 0, 0, 1800);
                for (auto i: pn) {
                    if ((x - (i.x + CURRAD)) * (x - (i.x + CURRAD)) + (y - (i.y + CURRAD)) * (y - (i.y + CURRAD)) <=
                        CURRAD * CURRAD) {
                        val = i.node->val;
                        break;
                    }
                }
            } else if (curt == 2) {
                auto pn = arrange_tree(avl.root, 0, 0, 1800);
                for (auto i: pn) {
                    if ((x - (i.x + CURRAD)) * (x - (i.x + CURRAD)) + (y - (i.y + CURRAD)) * (y - (i.y + CURRAD)) <=
                        CURRAD * CURRAD) {
                        val = i.node->val;
                        break;
                    }
                }
            } else if (curt == 3) {
                auto pn = arrange_tree(rbtree.root, 0, 0, 1800);
                for (auto i: pn) {
                    if ((x - (i.x + CURRAD)) * (x - (i.x + CURRAD)) + (y - (i.y + CURRAD)) * (y - (i.y + CURRAD)) <=
                        CURRAD * CURRAD) {
                        val = i.node->val;
                        break;
                    }
                }
            }
            if (val != -1337) {
                treap.erase(val);
                splay.erase(val);
                avl.erase(val);
                rbtree.erase(val);
                st.erase(val);
            }
        }
    }
}


void Handler::DrawTree() {
    w.clear();
    if (curt == 0) {
        drawtree(treap, w, rendervalues, renderpriority);
    } else if (curt == 1) {
        drawtree(splay, w, rendervalues, renderpriority);
    } else if (curt == 2) {
        drawtree(avl, w, rendervalues, renderpriority);
    } else if (curt == 3) {
        drawtree(rbtree, w, rendervalues, renderpriority);
    }
    if (rendertext) {
        command.drawbox(w);
    }
    w.display();
}

void Handler::OnResize() {
    sf::Vector2f size = static_cast<sf::Vector2f>(w.getSize());
    w.setSize(static_cast<sf::Vector2u>(size));
    auto mGUIView = sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y));
    w.setView(mGUIView);
}

