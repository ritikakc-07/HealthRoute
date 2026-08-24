#include "BSTAdmin.h"
#include <iostream>
#include <iomanip>

BSTAdmin::BSTAdmin() : root(nullptr), size(0) {}

BSTAdmin::~BSTAdmin() { destroy(root); }

void BSTAdmin::destroy(BSTNode* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ---- Insert ------------------------------------------------------------
void BSTAdmin::insert(const Hospital& h) {
    root = insertNode(root, h);
}

BSTNode* BSTAdmin::insertNode(BSTNode* node, const Hospital& h) {
    if (!node) {
        size++;
        return new BSTNode(h);
    }
    if (h.id < node->data.id) {
        node->left = insertNode(node->left, h);
    } else if (h.id > node->data.id) {
        node->right = insertNode(node->right, h);
    } else {
        node->data = h;   // duplicate id -> update existing record
    }
    return node;
}

// ---- Search --------------------------------------------------------------
Hospital* BSTAdmin::search(int id) { return searchNode(root, id); }

Hospital* BSTAdmin::searchNode(BSTNode* node, int id) {
    if (!node) return nullptr;
    if (id == node->data.id) return &node->data;
    return id < node->data.id ? searchNode(node->left, id) : searchNode(node->right, id);
}

// ---- Update ----------------------------------------------------------
bool BSTAdmin::updateICUBeds(int id, int available) {
    Hospital* h = search(id);
    if (!h) return false;
    h->availableICUBeds = available;
    return true;
}

// ---- Delete -------------------------------------------------------------
bool BSTAdmin::remove(int id) {
    bool removed = false;
    root = removeNode(root, id, removed);
    if (removed) size--;
    return removed;
}

BSTNode* BSTAdmin::findMin(BSTNode* node) {
    while (node->left) node = node->left;
    return node;
}

BSTNode* BSTAdmin::removeNode(BSTNode* node, int id, bool& removed) {
    if (!node) return nullptr;

    if (id < node->data.id) {
        node->left = removeNode(node->left, id, removed);
    } else if (id > node->data.id) {
        node->right = removeNode(node->right, id, removed);
    } else {
        removed = true;
        if (!node->left) {
            BSTNode* right = node->right;
            delete node;
            return right;
        }
        if (!node->right) {
            BSTNode* left = node->left;
            delete node;
            return left;
        }
        BSTNode* successor = findMin(node->right);
        node->data = successor->data;
        bool dummy = false;
        node->right = removeNode(node->right, successor->data.id, dummy);
    }
    return node;
}

// ---- Display / traversal -----------------------------------------------
void BSTAdmin::inorder(BSTNode* node, std::vector<Hospital>& out) const {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->data);
    inorder(node->right, out);
}

std::vector<Hospital> BSTAdmin::getAll() const {
    std::vector<Hospital> out;
    inorder(root, out);
    return out;
}

void BSTAdmin::displayAll() const {
    std::vector<Hospital> all = getAll();
    if (all.empty()) {
        std::cout << "  No hospital records found.\n";
        return;
    }
    std::cout << std::left
               << std::setw(5)  << "ID"
               << std::setw(26) << "Name"
               << std::setw(16) << "Location"
               << std::setw(10) << "Total"
               << std::setw(10) << "Available" << "\n";
    std::cout << std::string(67, '-') << "\n";
    for (const auto& h : all) {
        std::cout << std::left
                   << std::setw(5)  << h.id
                   << std::setw(26) << h.name
                   << std::setw(16) << h.location
                   << std::setw(10) << h.totalICUBeds
                   << std::setw(10) << h.availableICUBeds << "\n";
    }
}

int BSTAdmin::count() const { return size; }
