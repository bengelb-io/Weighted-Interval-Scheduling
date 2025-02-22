#include <iostream>
#include <vector>
#include <map>
#include <functional>
using namespace std;


template <typename T>
class Node {
private:
    Node<T>* prev = nullptr;
public:
    T value;
    Node<T>* left; 
    Node<T>* right;
    Node(T v) {
        value = v;
        left = nullptr;
        right = nullptr;
    }
    bool isLeaf() {
        return !left && !right;
    }
    bool hasLeaf() {
        return left == nullptr || right == nullptr;
    }
    // bool rightLeaning() {
    //     return right && !left;
    // }
    // bool leftLeaning() {
    //     return left && !right;
    // }
};

template <typename U>
class BinaryTree {
public:
    Node<U>* root;
    BinaryTree(Node<U>* r) {
        root = r;
    };
    int getHeight(Node<U>* n) {
        if (!n) {
            return 0;
        }
        if(n->isLeaf()) {
            return 1;
        }
        int lHeight = 1 + getHeight(n->left);
        int rHeight = 1 + getHeight(n->right);
        if (lHeight > rHeight) {
            return lHeight;
        } 
        return rHeight;
    }

    int accumulate(Node<U>* n, function<int(U)> callback) {
        if (!n) {
            return 0;
        }
        if (n->isLeaf()) {
            return callback(n->value);
        }
        int laccumalate = callback(n->value) + accumulate(n->left, callback);
        int raccumalate = callback(n->value) + accumulate(n->right, callback);
        if (laccumalate > raccumalate) {
            return laccumalate;
        }
        return raccumalate;
    }

    void treeShake(Node<U>* n, function<void(Node<U>*&)> callback) {
        if(!n) {
            return;
        }
        if(n->hasLeaf()) {
            callback(n);
        }
        treeShake(n->left, callback);
        treeShake(n->right, callback);
    }

    void climb(Node<U>* n, function<void(Node<U>*, int)> callback, int level = 0) {
        if (!n) {
            return;
        }
        callback(n, level);
        climb(n->left, callback, level+1);
        climb(n->right, callback, level+1);
    }

    map<int, vector<Node<U>*>> getLevels() {
        map<int,vector<Node<U>*>> levels;
        climb(root, [&levels] (Node<U>* n, int level) {
            auto it = levels.find(level);
            if(it == levels.end()) {
                levels[level] = vector<Node<U>*>({n});
            } else {
                levels[level].push_back(n);
            }
        });
        return levels;
    }

    vector<Node<U>*> getBranches() {
        vector<Node<U>*> leafs;
        treeShake(root, [&leafs](Node<U>*& n) {
            leafs.push_back(n);
        });
        return leafs;
    }

    void greedyInsert(U n, function<bool(U, U)> qualify) {
        vector<Node<U>*> leafs = getBranches();
        cout << "Leafs: " << leafs.size() << endl;
        for (Node<U>* leaf: leafs) {
            if(qualify(leaf->value, n)) {
                cout << "inserting node";
                Node<U>* insertNode = new Node<U>(n);
                if(!leaf->left) {
                    cout << " left";
                    leaf->left = insertNode;
                } else {
                    cout << " right";
                    leaf->right = insertNode;
                }
                cout << endl;
            }
        }
    }

    // void print(function<string(U)> repr) {
    //     treeShake(root, [] (Node<U>* n) {
    //         n->value;
    //     })
    // }
    
    
};

struct Appointment {
    int start;
    int end;
    int weight;
};


// void printAppointments(vector<Appointment> appts) {
//     cout << "[";
//     for (Appointment appt : appts) {
//         cout << appt.start << " - " << appt.end << ", ";
//     }
//     cout << "]" << endl;
// }

vector<Appointment> myAppointments = vector<Appointment>({
    {
        1,3,10
    },
    {
        2,5,25
    },
    {
        3,4,15
    },
    {
        5,7,10
    },
    {
        6,8,20
    }
});

// class Wis {
// public:
//     Wis(vector<Appointment> appts) {
//         BinaryTree<Appointment>* bt = new BinaryTree(
//             new Node<Appointment>(
//                 Appointment{
//                     0, 0, 0
//                 }
//             )
//         );
//         for(int i = 0; i < appts.size(); i++) {
//             Appointment appt = appts[i];
//             bt->greedyInsert(appt, [&appt] (Appointment prev, Appointment appt) {
//                 return prev.end < appt.start;
//             });
//         }
//     }
// };

void wis(vector<Appointment> appts) {
    BinaryTree<Appointment>* bt = new BinaryTree<Appointment>(
        new Node<Appointment>(
            Appointment{
                0, 0, 0
            }
        )
    );
    for(int i = 0; i < appts.size(); i++) {
        Appointment appt = appts[i];
        bt->greedyInsert(appt, [] (Appointment prev, Appointment appt) {
            cout <<"leaf node: {" << prev.start << "," << prev.end << ","  <<  prev.weight << "}" << endl;
            cout <<"insert node: {" << appt.start << "," << appt.end << ","  <<  appt.weight << "}" << endl;
            return prev.end <= appt.start;
        });
    }
    map<int, vector<Node<Appointment>*>>levels = bt->getLevels();
    for (auto it = levels.begin(); it != levels.end(); ++it) {
        cout << "Level: "<< it->first;
        auto apptsOnLevel = it->second;
        cout << "; Size: " << apptsOnLevel.size() << endl;

        for (auto appt : apptsOnLevel) {
            cout << appt->value.start << "-" << appt->value.end << endl;
        } 
    }
    int highestWeight = bt->accumulate(bt->root, [] (Appointment n) {
        return n.weight;
    });
    cout << highestWeight << endl;
    // cout << bt->getBranches().size() << endl;
}


int main() {
    int x = 4;
    int* px = &x;
    wis(myAppointments);


    
    return 0;
}