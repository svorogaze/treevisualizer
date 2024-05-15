#include <utility>
class RBtree {
public:
    struct Node {
        Node *l = nullptr, *r = nullptr;
        int sz = 0;
        int h = 0;
        long long val = 0;
        long long y  = -1;
        bool red = false;
        Node* par = nullptr;
        std::pair<int, int> width;
        Node(long long x) {
            val = x;
        }
    };

    Node *root = nullptr;


    bool exists(long long x);
    void insert(long long x);
    void erase(long long x);

    Node *find(Node *v, long long int key);

    bool getcolor(Node *v);

    Node *getmax(Node *v);

    Node *getmin(Node *v);

    void recalc(Node *v);

    Node *insert(Node *v, long long int key);

    Node *right(Node *x);

    Node *left(Node *x);

    void balanceinsert(Node *v);

    Node * getuncle(Node *v);

    Node *deletemin(Node *v);

    Node *fixup(Node *v);

    Node *redleft(Node *v);

    Node *redright(Node *v);

    Node *erase(Node *v, long long int key);
};