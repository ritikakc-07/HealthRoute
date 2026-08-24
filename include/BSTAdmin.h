#ifndef BST_ADMIN_H
#define BST_ADMIN_H

#include "Hospital.h"
#include <vector>

// Binary Search Tree node, keyed on Hospital::id.
struct BSTNode {
    Hospital data;
    BSTNode* left;
    BSTNode* right;
    explicit BSTNode(const Hospital& h) : data(h), left(nullptr), right(nullptr) {}
};

// Admin module: manages hospital records in a BST.
// Supports Insert, Search, Display (in-order), Update, Delete.
class BSTAdmin {
public:
    BSTAdmin();
    ~BSTAdmin();

    void insert(const Hospital& h);
    bool remove(int id);
    Hospital* search(int id);
    bool updateICUBeds(int id, int available);
    void displayAll() const;
    std::vector<Hospital> getAll() const;   // in-order traversal (sorted by id)
    int count() const;

private:
    BSTNode* root;
    int size;

    BSTNode* insertNode(BSTNode* node, const Hospital& h);
    BSTNode* removeNode(BSTNode* node, int id, bool& removed);
    BSTNode* findMin(BSTNode* node);
    Hospital* searchNode(BSTNode* node, int id);
    void inorder(BSTNode* node, std::vector<Hospital>& out) const;
    void destroy(BSTNode* node);
};

#endif
