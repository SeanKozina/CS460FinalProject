//
// Created by Spenc on 3/3/2024.
//

#ifndef PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H
#define PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H

#include "token.h"
#include "tokenizer.h"
#include "string"
#include "fstream"

using namespace std;

struct Node{
    //token tokenNode;
    Node* rightSibling;
    Node* leftChild;

    string tokenString;

};


class concreteSyntaxTree {
public:

    concreteSyntaxTree();

    void createConcreteSyntaxTree(tokenizer *treeTokenizer);

    void insertNodeRightSibling(Node* newNode);

    void insertNodeLeftChild(Node* newNode);

    void insertNodeFirst(Node* newNode);

    void printCST();




private:
    Node* first;
    Node* last;
    ofstream outputFile;
    int size;



};


#endif //PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H
