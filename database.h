#include "list.h"
#include "polynomial.h"
#include <string>
#include <iostream>
#include <fstream>
class database {
public:
    List<Polynomial> pl;
    database() {
        pl.plusequal = false;
    }

    void insert(Polynomial p) {
        pl.insert(p);
    }

    void erase(int ind) {
        pl.erase(ind);
    }

    void LoadFromFile(std::string s) {
        List<Polynomial> nw;
        std::swap(nw, pl);
        std::ifstream in(s);
        std::string a;
        while (in >> a) {
            insert(a);
        }
    }

    void SaveToFile(std::string s) {
        std::ofstream f(s);
        for (auto it = pl.begin(); it != pl.end(); it = it->next) {
            f << it->val.GetRepresentation() << '\n';
        }
    }

};