#include "BST.h"
#include <iostream>

using namespace std;

// ==============================
// Constructor
// ==============================

BST::BST()
{
    root = nullptr;
}

// ==============================
// Insert Hospital (Private)
// ==============================

Node* BST::insert(Node* root, Hospital hospital)
{
    if (root == nullptr)
    {
        return new Node(hospital);
    }

    if (hospital.id < root->data.id)
    {
        root->left = insert(root->left, hospital);
    }
    else if (hospital.id > root->data.id)
    {
        root->right = insert(root->right, hospital);
    }

    return root;
}

// ==============================
// Insert Hospital (Public)
// ==============================

void BST::insert(Hospital hospital)
{
    root = insert(root, hospital);
}

// ==============================
// Display Hospitals (Inorder)
// ==============================

void BST::inorder(Node* root)
{
    if (root == nullptr)
    {
        return;
    }

    inorder(root->left);

    cout << "----------------------------------" << endl;
    cout << "Hospital ID : " << root->data.id << endl;
    cout << "Name        : " << root->data.name << endl;
    cout << "Location    : " << root->data.location << endl;
    cout << "ICU Beds    : " << root->data.icuBeds << endl;

    inorder(root->right);
}

// ==============================
// Display Hospitals (Public)
// ==============================

void BST::displayHospitals()
{
    if (root == nullptr)
    {
        cout << "\nNo hospital records found.\n";
        return;
    }

    inorder(root);
}

// ==============================
// Search Hospital (Private)
// ==============================

Node* BST::search(Node* root, int id)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (root->data.id == id)
    {
        return root;
    }

    if (id < root->data.id)
    {
        return search(root->left, id);
    }

    return search(root->right, id);
}

// ==============================
// Search Hospital (Public)
// ==============================

void BST::search(int id)
{
    Node* result = search(root, id);

    if (result == nullptr)
    {
        cout << "\nHospital not found!\n";
    }
    else
    {
        cout << "\nHospital Found!\n";
        cout << "----------------------------------" << endl;
        cout << "Hospital ID : " << result->data.id << endl;
        cout << "Name        : " << result->data.name << endl;
        cout << "Location    : " << result->data.location << endl;
        cout << "ICU Beds    : " << result->data.icuBeds << endl;
    }
}
// ==============================
// Update ICU Beds (Private)
// ==============================

Node* BST::updateICUBeds(Node* root, int id, int beds)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (root->data.id == id)
    {
        root->data.icuBeds = beds;
        return root;
    }

    if (id < root->data.id)
    {
        return updateICUBeds(root->left, id, beds);
    }

    return updateICUBeds(root->right, id, beds);
}

// ==============================
// Update ICU Beds (Public)
// ==============================

void BST::updateICUBeds(int id, int beds)
{
    Node* result = updateICUBeds(root, id, beds);

    if (result == nullptr)
    {
        cout << "\nHospital not found!\n";
    }
    else
    {
        cout << "\nICU Beds Updated Successfully!\n";
    }
}

// ==============================
// Find Minimum Node
// ==============================

Node* BST::findMin(Node* root)
{
    while (root != nullptr && root->left != nullptr)
    {
        root = root->left;
    }

    return root;
}

// ==============================
// Delete Hospital (Private)
// ==============================

Node* BST::deleteHospital(Node* root, int id)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (id < root->data.id)
    {
        root->left = deleteHospital(root->left, id);
    }
    else if (id > root->data.id)
    {
        root->right = deleteHospital(root->right, id);
    }
    else
    {
        // Case 1: No Child
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            return nullptr;
        }

        // Case 2: One Child
        if (root->left == nullptr)
        {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        if (root->right == nullptr)
        {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // Case 3: Two Children
        Node* temp = findMin(root->right);

        root->data = temp->data;

        root->right = deleteHospital(root->right, temp->data.id);
    }

    return root;
}

// ==============================
// Delete Hospital (Public)
// ==============================

void BST::deleteHospital(int id)
{
    if (search(root, id) == nullptr)
    {
        cout << "\nHospital not found!\n";
        return;
    }

    root = deleteHospital(root, id);

    cout << "\nHospital Deleted Successfully!\n";
}

// ==============================
// Store Hospitals in Vector
// ==============================

void BST::storeHospitals(Node* root, vector<Hospital>& hospitals)
{
    if (root == nullptr)
    {
        return;
    }

    storeHospitals(root->left, hospitals);

    hospitals.push_back(root->data);

    storeHospitals(root->right, hospitals);
}

// ==============================
// Get All Hospitals
// ==============================

vector<Hospital> BST::getAllHospitals()
{
    vector<Hospital> hospitals;

    storeHospitals(root, hospitals);

    return hospitals;
}