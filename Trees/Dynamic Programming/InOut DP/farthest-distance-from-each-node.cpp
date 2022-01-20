/**
 * Given a tree, for each node, output the distance to the node farthest from
 * it.
 *
 * Example: 1->2, 1->3
 * Output: 1 2 2
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

// Creating an n-ary tree as given in the above link.
Node* Tree::createTree() {
    // create all nodes first;
    for (int64_t i = 1; i <= 19; i++) nodes[i] = new Node(i);

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

const int64_t NUM = 21;
std::vector<int64_t> IN(NUM, 0), OUT(NUM, 0);

void calculateIN(Node* root) {
    int64_t ans = 0;
    for (auto child : root->children) {
        calculateIN(child);
        ans = std::max(ans, IN[child->data] + 1);
    }

    IN[root->data] = ans;
}

// Calculated using my parent's OUT and my siblings IN, along with some
// modifications as per question requirement.
// OUT of actual root is always 0.
void calculateOUT(Node* root) {
    int64_t max1 = -1, max2 = -1;
    for (auto child : root->children) {
        if (IN[child->data] > max1)
            max2 = max1, max1 = IN[child->data];
        else if (IN[child->data] > max2)
            max2 = IN[child->data];
    }

    for (auto child : root->children) {
        auto siblingMax = IN[child->data] == max1 ? max2 : max1;
        OUT[child->data] = std::max(1 + OUT[root->data], 2 + siblingMax);
        calculateOUT(child);
    }
}

int main() {
    Node* root = Tree::createTree();

    // Calculate IN dp. It represents the max height possible in my subtree.
    calculateIN(root);

    // Calculate OUT dp. It represents the max height possible outside of the
    // scope of my subtree.
    calculateOUT(root);

    // Calculate ans of all nodes.
    for (int i = 1; i <= 19; i++) std::cout << std::max(IN[i], OUT[i]) << " ";

    std::cout << std::endl;
}