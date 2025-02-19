#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

struct Appointment {
    int start;
    int end;
    int weight;
};


class Branch {
private:
    int distance_from_root;
public:
    Branch(int v) {
        value = v;
    }
    int value;
    Branch* left = nullptr;
    Branch* right = nullptr;
    void setLeft(Branch b) {
        left = &b;
    }
    void setRight(Branch b) {
        right = &b;
    }
};

class Tree {
public:
    Tree(int v) {
        root = new Branch(v);
    }
    Branch* root;
    Branch* nextBranch(bool segment, Branch* cursor) {
        if(segment) {
            return cursor->left;
        }
        return cursor->right;

    }
    bool validRoute(vector<bool> route) {
        Branch* cursor = root;
        for (int i = 0; i < route.size(); i++) {
            bool segment = route[i];
            Branch* b = nextBranch(segment, cursor);
            if(!b) {
                return false;
            }
        }
        return true;
    }
    Branch* traverse(vector<bool> route) {
        Branch* cursor = root;
        if(!validRoute(route)) {
            return nullptr;
        }
        for (int i = 0; i < route.size(); i++) {
            bool segment = route[i];
            Branch* b = nextBranch(segment, cursor);
            cursor = b;
        }
    }
};

bool L = true;
bool R = false; 

int main() {
    cout << "hi" << endl;
}