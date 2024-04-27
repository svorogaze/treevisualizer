#include "database.h"
#include "string"
#include "sstream"
#include "iostream"

class Handler {
    database db;
    Polynomial cur;
    bool haspol = false;
public:

    Handler() {
        db.pl.ignorezero = false;
    }

    void HandleEvent() {
        std::string s;
        getline(std::cin, s);
        if (s.empty()) {
            return;
        }
        std::stringstream ss(s);
        std::string command;
        ss >> command;
        if (command == "insert") {
            try {
                std::string poly;
                ss >> poly;

                Polynomial p(poly);
                db.insert(p);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "remove") {
            try {
                int ind;
                ss >> ind;
                db.erase(ind);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "save") {
            try {
                std::string filename;
                ss >> filename;
                db.SaveToFile(filename);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "load") {
            try {
                std::string filename;
                ss >> filename;
                db.LoadFromFile(filename);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "push") {
            try {
                if (!haspol) throw std::runtime_error("don't have current polynomial");
                db.insert(cur);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "copy") {
            try {
                int ind;
                ss >> ind;
                db.insert(db.pl[ind].val);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "mul") {
            try {
                int a, b;
                ss >> a >> b;
                Polynomial res = db.pl[a].val * db.pl[b].val;
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "div") {
            try {
                int a, b;
                ss >> a >> b;
                Polynomial res = db.pl[a].val / db.pl[b].val;
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "add") {
            try {
                int a, b;
                ss >> a >> b;
                Polynomial res = db.pl[a].val + db.pl[b].val;
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "sub") {
            try {
                int a, b;
                ss >> a >> b;
                Polynomial res = db.pl[a].val - db.pl[b].val;
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "mod") {
            try {
                int a, b;
                ss >> a >> b;
                Polynomial res = db.pl[a].val % db.pl[b].val;
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "clear") {
            try {
                List<Polynomial> pl;
                std::swap(pl, db.pl);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "deriative") {
            try {
                char c;
                int ind, times;
                ss >> ind >> c >> times;
                Polynomial res = db.pl[ind].val.GetXDeriative(c, times);
                std::cout << "result: " << res.GetRepresentation() << std::endl;
                cur = res;
                haspol = true;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "roots") {
            try {
                int ind;
                ss >> ind;
                auto res = db.pl[ind].val.GetRoots();
                std::cout << "Found roots: ";
                for (auto i: res) {
                    std::cout << i << ' ';
                }
                std::cout << std::endl;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "value") {
            try {
                int ind;
                ss >> ind;
                std::vector<long double> val(26);
                char c;
                long double x;
                std::vector<bool> bl(26);
                for (int i = 0; i < 26; ++i) {
                    if (db.pl[ind].val.IsPresent('a' + i)) bl[i] = true;
                }
                while (ss >> c >> x) {
                    if (c < 'a' || c > 'z' || !db.pl[ind].val.IsPresent(c)) {
                        throw std::runtime_error("incorrect char");
                    }
                    val[c - 'a'] = x;
                    bl[c - 'a'] = false;
                }
                for (int i = 0; i < 26; ++i) {
                    if (bl[i]) {
                        throw std::runtime_error("char present in polynomial not present in query");
                    }
                }
                std::cout << db.pl[ind].val.GetValue(val) << std::endl;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "equal") {
            try {
                int ind1, ind2;
                ss >> ind1 >> ind2;
                if (db.pl[ind1].val == db.pl[ind2].val) {
                    std::cout << "equal" << std::endl;
                } else {
                    std::cout << "not equal" << std::endl;
                }
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "help") {
            std::cout << "insert x - insert polynomial x in database\n"
                         "remove i - remove polynomial on index i\n"
                         "save filename - save database to filename\n"
                         "load filename - load database from filename\n"
                         "push - insert result from previous operation to database\n"
                         "copy ind - insert polynomial from index i\n"
                         "mul ind1 ind2 - multiply polynomials at index ind1 and ind2\n"
                         "div ind1`ind2 - divide polynomials from index ind1 by polynomial from index ind2\n"
                         "add ind1 ind2 - add polynomials from index ind1 and ind2\n"
                         "sub ind1 ind2 - substract polynomial from index ind1 polynomial from index ind2\n"
                         "mod ind1 ind2 - get mod of ind1 by mod2\n"
                         "clear - erase all polynomials from database\n"
                         "deriative ind char x - get xth deriative of char of polynomial from index i\n"
                         "roots ind - find integer roots of polynomial from index ind\n"
                         "value char1 x1... - find value of polynomial in point(x1, x2...)\n"
                         "equal ind1 ind2 check if polynomials from ind1 and ind2 are equal" << std::endl;
        } else {
            std::cout << "incorrect command!\n";
        }
    }

    void PrintPolynomials() {
        for (int i = 0; i < db.pl.size(); ++i) {
            std::cout << i << ": " << db.pl[i].val.GetRepresentation() << std::endl;
        }
    }


};