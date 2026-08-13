#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include "Hospital.h"

using namespace std;

struct Node
{
    Hospital data;
    Node* left;
    Node* right;

    Node(Hospital hospital)
    {
        data = hospital;
        left = nullptr;
        right = nullptr;
    }
};

class BST
{
private:
    Node* root;

    // Private Functions
    Node* insert(Node* root, Hospital hospital);

    void inorder(Node* root);

    Node* search(Node* root, int id);

    Node* updateICUBeds(Node* root, int id, int beds);

    Node* deleteHospital(Node* root, int id);

    Node* findMin(Node* root);

    void storeHospitals(Node* root, vector<Hospital>& hospitals);

public:

    BST();

    // Insert Hospital
    void insert(Hospital hospital);

    // Display Hospitals
    void displayHospitals();

    // Search Hospital
    void search(int id);

    // Update ICU Beds
    void updateICUBeds(int id, int beds);

    // Delete Hospital
    void deleteHospital(int id);

    // Return all hospitals for Merge Sort
    vector<Hospital> getAllHospitals();
};

#endif