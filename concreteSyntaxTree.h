//
// Created by Spenc on 3/3/2024.
//

#ifndef PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H
#define PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H

#include <fstream>
#include <string>

using std::string;

struct Node{
    Node* rightSibling;
    Node* leftChild;

    string tokenString;
    int onLine;
public:

};

class concreteSyntaxTree {
public:
    concreteSyntaxTree();

    void createConcreteSyntaxTree(std::ifstream *inFile, std::ofstream *outFile, std::string TOKENFILE);

    void insertNodeRightSibling(Node* newNode);

    void insertNodeLeftChild(Node* newNode);

    void insertNodeFirst(Node* newNode);

    void printCST();

    void printST(Node* next, std::ofstream* outFile);
    void CheckCST(Node* next, std::ofstream* outFile, std::string TOKENFILE);
    
    void SetError(std::ofstream* outFile, std::string TOKENFILE);
    bool CheckForReservedTypes(std::string inString);


private:
    Node* first;
    Node* last;
    std::ofstream outputFile;
    int size;

    int offset;
    int lineCounter;
    int errorAssign;
    int errorOnLine;
    string offsetString;
    string invalidString;

    bool bvalidST;
};


#endif //PROGRAMMINGASSIGNMENT1_CONCRETESYNTAXTREE_H
