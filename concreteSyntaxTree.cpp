//
// Created by Spenc on 3/3/2024.
//

#include <iostream>

#include "concreteSyntaxTree.h"
#include "tokenizer.h"


using namespace std;

concreteSyntaxTree::concreteSyntaxTree() {
    size = 0;
    last = nullptr;
    first = nullptr;
    offset = 0;
    offsetString = "";
    lineCounter = 0;
    bvalidST = true;
}

void concreteSyntaxTree::createConcreteSyntaxTree(std::ifstream* inFile, std::ofstream* outFile, std::string TOKENFILE) {

    int lineCount = 1;

    tokenizer tokenizerV;
    std::string mytoken = tokenizerV.GetCharacterForConcreteSyntax(inFile, outFile, lineCount);

    //allocate new memory for node
    Node* newNode = new Node;
    newNode->rightSibling = nullptr;
    newNode->leftChild = nullptr;
    newNode->tokenString = mytoken;

    if (mytoken != "/end/")
    {
        insertNodeFirst(newNode);
    }

    while (mytoken != "/end/")
    {
        mytoken = tokenizerV.GetCharacterForConcreteSyntax(inFile, outFile, lineCount);

        Node* nextNode = new Node;
        nextNode->rightSibling = nullptr;
        nextNode->leftChild = nullptr;
        nextNode->tokenString = mytoken;

        if (mytoken == "{" || mytoken == "}")
        {
            last->leftChild = nextNode;
            //lineCount += 2;
            nextNode->onLine = lineCount;
            insertNodeLeftChild(nextNode);

        }
        else
        {
            nextNode->onLine = lineCount;
            insertNodeRightSibling(nextNode);
        }

    }

    CheckCST(first, outFile, TOKENFILE);

    if (!bvalidST)
    {
        SetError(outFile, TOKENFILE);
    }
    else {
        printST(first, outFile);
    }
}

void concreteSyntaxTree::insertNodeRightSibling(Node* newNode) 
{
    Node* tempPtr = last;
    tempPtr->rightSibling = newNode;
    last = newNode;
    size++;

}

void concreteSyntaxTree::insertNodeLeftChild(Node *newNode) 
{
    Node* tempNode = last;
    tempNode->leftChild = newNode;
    last = newNode;
    size++;

}

void concreteSyntaxTree::insertNodeFirst(Node *newNode) {

    if(first == nullptr){
        first = newNode;

        size++;
        if(size == 1){
            last = newNode;
        }

    }else{
        exit(-1);
    }

}

void concreteSyntaxTree::printST(Node* next, std::ofstream* outFile)
{
    if (next == nullptr)
    {
        return;
    }

    string temp = next->tokenString;

    if (temp == "{" || temp == "}")
    {
        offset += offsetString.length();
        offset += 5; // add 5 more spaces 
        offsetString = "";

        *outFile << "\n";
        for (int i = 0; i < offset; i++)
        {
            *outFile << " "; 
        }
        *outFile << temp;
        *outFile << "\n";
        for (int i = 0; i < offset; i++)
        {
            *outFile << " "; 
        }        

    }

    else if (temp == ";")
    {
        offset += offsetString.length();
        offsetString = "";
        *outFile << temp;
        *outFile << "\n";
        for (int i = 0; i < offset; i++)
        {
            *outFile << " "; // or *outFile << "\t"; for tab spaces
        }
    }
    else {        
        if (next->rightSibling != nullptr)
        {
            *outFile << temp << " ------> ";
            offsetString += (temp + "------> ");
        }else{
            *outFile << temp;
            offsetString += temp;
        }

    }
    printST(next->rightSibling, outFile);
    printST(next->leftChild, outFile);

}


void concreteSyntaxTree::CheckCST(Node* next, std::ofstream* outFile, std::string TOKENFILE)
{
    if (next == nullptr)
    {
        return;
    }

    if (next->tokenString == "{" || next->tokenString == ";" || next->tokenString == "}")
    {
        lineCounter++;
    }

    if (next->rightSibling != nullptr) // check negative arrays
    {
        if (next->tokenString == "[" && next->rightSibling->tokenString == "-")
        {
            bvalidST = false;
            errorAssign = 1;
            errorOnLine = next->onLine;
            return;
        }

    }
    if (next->rightSibling != nullptr) // check for reserved types
    {
        if (CheckForReservedTypes(next->tokenString) && CheckForReservedTypes(next->rightSibling->tokenString))
        {
            bvalidST = false;
            errorAssign = 2;
            errorOnLine = next->onLine;
            return;
        }
    }


    if (bvalidST)
    {
        CheckCST(next->rightSibling, outFile, TOKENFILE);
    }
    if (bvalidST)
    {
        CheckCST(next->leftChild, outFile, TOKENFILE);
    }

}

void concreteSyntaxTree::SetError(std::ofstream* outFile, std::string TOKENFILE)
{
    switch (errorAssign)
    {
        case 1: // negative array
            outFile->close();
            outFile->open(TOKENFILE + "_SyntaxTree.txt");
            *outFile << "Syntax error on line " << errorOnLine << " array size must be declared as a positive integer" << std::endl;
            break;
    
        case 2: // reserved words
            outFile->close();
            outFile->open(TOKENFILE + "_SyntaxTree.txt");
            *outFile << "Syntax error on line " << errorOnLine << ": reserved word " << invalidString << " cannot be for the name of a variable" << std::endl;
            break;

        default:
            break;
    }

}

bool concreteSyntaxTree::CheckForReservedTypes(std::string inString)
{
    const std::string reservedTypes[] = {
    "bool",
    "char",
    "char8_t",      // Since C++20
    "char16_t",
    "char32_t",
    "wchar_t",
    "short",
    "int",
    "long",
    "long long",
    "float",
    "double",
    "long double",
    "void",
    "auto",         // Since C++11
    "decltype",     // Since C++11
    "nullptr",      // Since C++11
    "signed",
    "unsigned",
    "const",
    "volatile",
    "static",
    "extern",
    "register",
    "mutable",
    "thread_local", // Since C++11
    "constexpr",    // Since C++11
    "consteval",    // Since C++20
    "constinit",    // Since C++20
    "inline",
    "virtual",
    "explicit",
    "friend",
    "private",
    "protected",
    "public",
    "typedef",
    "typename",
    "using",        // Since C++11
    "template",
    "namespace",
    "operator",
    "class",
    "struct",
    "enum",
    "union"
    };

    for (auto item : reservedTypes)
    {
        if (item == inString)
        {
            invalidString = item;
            return true;
        }
    }
    
    return false;
}




void concreteSyntaxTree::printCST() 
{
//    outputFile.open("CST.txt");
//
//    Node* temp = first;
//    Node* prev = first;
//
//    bool endloop = false;
//
//    int listLength = 0;
//
//
//    while(!endloop) {
//
////        int tempLengthStart = listLength;
////
////        while(tempLengthStart != 0){
////            cout << " ";
////            tempLengthStart--;
////            //listLength++;
////
////        }
//
//        while (temp != nullptr) {
//
//
//
//            //cout << temp->tokenString << " --> ";
//            outputFile << temp->tokenString << " --> ";
//
//           // listLength = listLength + temp->tokenString.size() + 5;
//
//
//            prev = temp;
//            temp = temp->rightSibling;
//
//        }
//
//
//        if (prev != nullptr)
//        {
//            Node *lastNode = prev;
//            prev = prev->leftChild; 
//        }
//
//
//        while (prev != nullptr) {
//
//            int tempLength = listLength;
//
//            outputFile << '\n';
//
////            while(tempLength != 0){
////                cout << " ";
////                tempLength--;
////                //listLength++;
////            }
//
//            //cout << "\n | \n" << prev->tokenString;
//            outputFile << " | \n";
//            //listLength = listLength + 3 + prev->tokenString.size();
////
////            int testlen = listLength;
////
////            while(testlen != 0){
////                cout << " ";
////                testlen--;
////            }
//
//
//            outputFile << prev->tokenString;
//
//
//            temp = prev;
//            prev = prev->leftChild;
//
//        }
//
//        outputFile << " --> ";
//
//        if (temp != nullptr) {
//            if (temp->rightSibling == nullptr && temp->leftChild == nullptr) {
//                endloop = true;
//            }
//            temp = temp->rightSibling;
//        }
//        
//
//
//    }

}


