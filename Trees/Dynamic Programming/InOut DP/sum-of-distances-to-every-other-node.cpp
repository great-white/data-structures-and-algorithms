/**
 * Given a tree, for each node, find the sum of distances to every other node,
 * in linear time.
 *
 * Example: 1->2, 1->3
 * Output: 2 3 3
 *
 * Ref: https://blogarithms.github.io/articles/2019-10/inout-dp-tree
 **/

#include <bits/stdc++.h>

class Node {
   public:
    int64_t data;
    std::vector<Node*> children;

    Node(int64_t data) { this->data = data; }
};

class Tree {
   private:
    static std::map<int64_t, Node*> nodes;

    static void addChildren(int64_t parent, int64_t numOfChildren, ...) {
        va_list children;
        va_start(children, numOfChildren);

        while (numOfChildren--) {
            auto child = va_arg(children, int64_t);
            nodes[parent]->children.push_back(nodes[child]);
        }

        va_end(children);
    }

   public:
    static Node* createTree();
};

// Initialize static variables.
std::map<int64_t, Node*> Tree::nodes;

const int64_t NUM = 19;

// Creating an n-ary tree as given in the above link.
Node* Tree::createTree() {
    // create all nodes first;
    for (int64_t i = 1; i <= NUM; i++) nodes[i] = new Node(i);

    // 1's children are 2, 3, 4
    addChildren(1, 3, 2, 3, 4);

    // 2's children are 5, 6, 7
    addChildren(2, 3, 5, 6, 7);

    // 3's children are 8, 9
    addChildren(3, 2, 8, 9);

    // 5,s children are 10, 11
    addChildren(5, 2, 10, 11);

    // 6's children are 12, 13
    addChildren(6, 2, 12, 13);

    // 7's children are 14, 15
    addChildren(7, 2, 14, 15);

    // 8's children are 16, 17
    addChildren(8, 2, 16, 17);

    // 9's children are 18, 19
    addChildren(9, 2, 18, 19);

    // 1 is the root.
    return nodes[1];
};

std::vector<int64_t> IN(NUM + 1, 0);
std::vector<int64_t> OUT(NUM + 1, 0);
std::vector<int64_t> NUM_CHILDREN(NUM + 1, 0);

void calculateIN(Node* root) {
    for (auto child : root->children) {
        calculateIN(child);
        NUM_CHILDREN[root->data] += 1 + NUM_CHILDREN[child->data];
        IN[root->data] += 1 + NUM_CHILDREN[child->data] + IN[child->data];
    }
}

void calculateOUT(Node* root) {
    for (auto child : root->children) {
        auto nodesIncludingMeAndMyChildren = NUM_CHILDREN[child->data] + 1;
        auto nodesExcludingMeAndMyChildren =
            NUM - NUM_CHILDREN[child->data] - 1;

        // Need to add "excluding" coz distance of all nodes from parent will
        // increase by 1.
        OUT[child->data] =
            nodesExcludingMeAndMyChildren + OUT[root->data] + IN[root->data];

        // You have also included my subtree. So, cancel it.
        OUT[child->data] -= IN[child->data];

        // But now distance of each node of my subtree from my parent remains
        // intact. Kindly subtract that as well. You alredy know how many nodes
        // are there in my subtree. ;)
        OUT[child->data] -= nodesIncludingMeAndMyChildren;

        // Oh, don't forget to recur on me.
        calculateOUT(child);
    }
}

int main() {
    Node* root = Tree::createTree();

    calculateIN(root);
    calculateOUT(root);

    // IN + OUT will give you the answer.
    for (int i = 1; i <= NUM; i++) std::cout << IN[i] + OUT[i] << " ";

    std::cout << std::endl;

    return 0;
}