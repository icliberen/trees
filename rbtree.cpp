/*
 * rbtree.cpp
 * Implementation of Red-Black Tree with DFS timestamps and interactive commands.
 *
 * How to Compile and Run:
 *		g++ rbtree.cpp -std=c++11 -o rbtree.exe
 *		./rbtree.exe input.txt
 *
 * Known Bugs and Limitations:
 *		Assumes input file contains one valid, unique string per line.
 *		Does not handle malformed commands beyond printing "Invalid Operation".
 */
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    string name;
    Color color;
    Node *left, *right, *parent;
    int discovery_time;
    int finish_time;
    Node(const string &s, Node *nil)
        : name(s), color(RED), left(nil), right(nil), parent(nil), discovery_time(0), finish_time(0) {}
};

class RBTree {
private:
    Node *root;
    Node *nil;
    int timer;
    vector<Node*> dfsList;

    void leftRotate(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != nil) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node *z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node *y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node *y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node *u, Node *v) {
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    Node* treeMinimum(Node *x) {
        while (x->left != nil) x = x->left;
        return x;
    }

    void deleteFixup(Node *x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node *w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void dfsHelper(Node *node) {
        if (node == nil) return;
        node->discovery_time = timer++;
        dfsList.push_back(node);
        dfsHelper(node->left);
        dfsHelper(node->right);
        node->finish_time = timer++;
    }

    Node* queryHelper(Node *node, int d, int f) {
        if (node == nil) return nullptr;
        if (node->discovery_time == d && node->finish_time == f) return node;
        Node *res = queryHelper(node->left, d, f);
        return res ? res : queryHelper(node->right, d, f);
    }

    void printRBTHelper(Node *node, int depth) {
        if (node == nil) return;
        string indent;
        for (int i = 0; i < depth; ++i) indent += ". ";
        cout << indent << node->name
             << " (" << (node->color == BLACK ? "BLACK" : "RED") << ")" << endl;
        printRBTHelper(node->left, depth + 1);
        printRBTHelper(node->right, depth + 1);
    }

    Node* searchNode(Node *node, const string &key) {
        if (node == nil) return nullptr;
        if (key == node->name) return node;
        return (key < node->name)
            ? searchNode(node->left, key)
            : searchNode(node->right, key);
    }

public:
    RBTree() {
        nil = new Node("", nullptr);
        nil->color = BLACK;
        nil->left = nil->right = nil->parent = nil;
        root = nil;
        timer = 1;
    }

    void insert(const string &key) {
        Node *z = new Node(key, nil);
        Node *y = nil;
        Node *x = root;
        while (x != nil) {
            y = x;
            x = (key < x->name) ? x->left : x->right;
        }
        z->parent = y;
        if (y == nil) root = z;
        else if (key < y->name) y->left = z;
        else y->right = z;
        z->left = z->right = nil;
        z->color = RED;
        insertFixup(z);
    }

    bool insertUnique(const string &key) {
        if (searchNode(root, key) != nullptr) return false;
        insert(key);
        return true;
    }

    bool deleteNode(const string &key) {
        Node *z = searchNode(root, key);
        if (!z) return false;
        Node *y = z;
        Color yOrigColor = y->color;
        Node *x;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = treeMinimum(z->right);
            yOrigColor = y->color;
            x = y->right;
            if (y->parent == z) x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOrigColor == BLACK) deleteFixup(x);
        return true;
    }

    void dfs() {
        timer = 1;
        dfsList.clear();
        dfsHelper(root);
    }

    void printDFS() {
        for (auto n : dfsList) {
            cout << n->name << " (" << (n->color == BLACK ? "BLACK" : "RED")
                 << "): d=" << n->discovery_time
                 << ", f=" << n->finish_time << endl;
        }
    }

    void query(int d, int f) {
        Node *res = queryHelper(root, d, f);
        if (res) cout << "Output: Node " << res->name
                      << " (" << (res->color == BLACK ? "BLACK" : "RED") << ")" << endl;
        else cout << "Invalid Operation" << endl;
    }

    void printRBT() {
        printRBTHelper(root, 0);
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " input.txt\n";
        return 1;
    }
    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "Cannot open input file.\n";
        return 1;
    }

    RBTree tree;
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) tree.insertUnique(line);
    }
    fin.close();

    while (true) {
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        cout << endl;
        istringstream iss(line);
        string cmd;
        iss >> cmd;
        cout << "Directive-----------------> " << line << endl;
        if (cmd == "insert") {
            string x;
            if (!(iss >> x) || !tree.insertUnique(x)) cout << "Invalid Operation" << endl;
        } else if (cmd == "delete") {
            string x;
            if (!(iss >> x) || !tree.deleteNode(x)) cout << "Invalid Operation" << endl;
        } else if (cmd == "dfs") {
            tree.dfs();
            tree.printDFS();
        } else if (cmd == "query") {
            int d, f;
            if (!(iss >> d >> f)) cout << "Invalid Operation" << endl;
            else tree.query(d, f);
        } else if (cmd == "print-rbt") {
            tree.printRBT();
        } else if (cmd == "quit") {
            break;
        } else {
            cout << "Invalid Operation" << endl;
        }
    }
    return 0;
}