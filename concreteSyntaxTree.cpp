//
// Created by Spenc on 3/3/2024.
//

#include "concreteSyntaxTree.h"
#include "iostream"

using namespace std;

concreteSyntaxTree::concreteSyntaxTree() {
    size = 0;
    last = nullptr;
    first = nullptr;
}

void concreteSyntaxTree::createConcreteSyntaxTree(tokenizer *treeTokenizer) {

    int lineCount = 0;

    token mytoken = treeTokenizer->getToken();

    //allocate new memory for node
    Node* newNode = new Node;
    newNode->rightSibling = nullptr;
    newNode->leftChild = nullptr;
    newNode->tokenString = mytoken.getToken();

    insertNodeFirst(newNode);

    //bool nextTokenLeftChild = false;

    lineCount = mytoken.getLineCount();


    while(!treeTokenizer->endOfFile()){




        mytoken = treeTokenizer->getToken();
        Node* nextNode = new Node;
        nextNode->rightSibling = nullptr;
        nextNode->leftChild = nullptr;
        nextNode->tokenString = mytoken.getToken();


        if(lineCount == mytoken.getLineCount()){
            insertNodeRightSibling(nextNode);

        }
        else{
            insertNodeLeftChild(nextNode);
        }

        lineCount = mytoken.getLineCount();




    }

}

void concreteSyntaxTree::insertNodeRightSibling(Node* newNode) {

    Node* tempPtr = last;
    tempPtr->rightSibling = newNode;
    last = newNode;
    size++;


}

void concreteSyntaxTree::insertNodeLeftChild(Node *newNode) {

    Node* tempNode = last;
    tempNode->leftChild = newNode;
    last = newNode;
    size++;


}

void concreteSyntaxTree::insertNodeFirst(Node *newNode) {

    if(first == nullptr){
    //newNode->rightSibling = first;
        first = newNode;

        size++;
        if(size == 1){
            last = newNode;
        }

    }else{
        exit(-1);
    }

}

void concreteSyntaxTree::printCST() {

    outputFile.open("CST.txt");

    Node* temp = first;
    Node* prev = first;

    bool endloop = false;

    int listLength = 0;


    while(!endloop) {

//        int tempLengthStart = listLength;
//
//        while(tempLengthStart != 0){
//            cout << " ";
//            tempLengthStart--;
//            //listLength++;
//
//        }

        while (temp != nullptr) {



            //cout << temp->tokenString << " --> ";
            outputFile << temp->tokenString << " --> ";

           // listLength = listLength + temp->tokenString.size() + 5;


            prev = temp;
            temp = temp->rightSibling;

        }

        Node *lastNode = prev;
        prev = prev->leftChild;



        while (prev != nullptr) {

            int tempLength = listLength;

            outputFile << '\n';

//            while(tempLength != 0){
//                cout << " ";
//                tempLength--;
//                //listLength++;
//            }

            //cout << "\n | \n" << prev->tokenString;
            outputFile << " | \n";
            //listLength = listLength + 3 + prev->tokenString.size();
//
//            int testlen = listLength;
//
//            while(testlen != 0){
//                cout << " ";
//                testlen--;
//            }


            outputFile << prev->tokenString;


            temp = prev;
            prev = prev->leftChild;

        }

        outputFile << " --> ";


        if(temp->rightSibling == nullptr && temp->leftChild == nullptr){
            endloop = true;
        }

        temp = temp->rightSibling;

    }

}


