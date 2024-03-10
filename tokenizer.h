//
// Created by Spenc on 2/21/2024.
//

#ifndef PROGRAMMINGASSIGNMENT1_TOKENIZER_H
#define PROGRAMMINGASSIGNMENT1_TOKENIZER_H
#include "fstream"
#include "token.h"

using namespace std;

class tokenizer {

public:
    //parameterized constructor
    tokenizer(const string& inputFile);

    //function for reading tokens from a file
    token getToken();

    string charOfInterest(char c);

    bool checkCharInterest(char c);

    bool endOfFile();

    bool isEOF();

    bool isPrevTokenEndline();


private:
    ifstream _inputStream;
    ofstream  _outputStream;

    string fileName;

    token prevToken;

    int lineCount;

    bool endFile;

};


#endif //PROGRAMMINGASSIGNMENT1_TOKENIZER_H
