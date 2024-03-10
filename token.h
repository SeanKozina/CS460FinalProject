//
// Created by Spencer on 2/20/2024.
//

#ifndef PROGRAMMINGASSIGNMENT1_TOKEN_H
#define PROGRAMMINGASSIGNMENT1_TOKEN_H
#include <string>

using namespace std;

class token {

public:

    //parameterized constructor
    token();
    //token(string type, string token);
    ~token();

    //setters
    void setTokenType(const string& tokenType);
    void setToken(const string& token);

    //getters for attributes of class
    string getTokenType();
    string getToken();

    void quotedString();
    bool isQuotedSTR();


    void singleQuote();
    bool isSingleQuote();


    void stringPrev();
    bool isStringPrev();

    void endQuote();
    bool isQuoteEnd();

    void tokenIsEOF();
    bool getTokenEOF();

    void incorrectSyntax();
    bool syntaxError();

    void setSyntaxMessage(string msg);
    string returnSyntaxmsg();

    void printToken();



    void setCommma();
    bool isComma();

    void setLeftParenthesis();
    bool isLeftParenthesis();

    void setRightParenthesis();
    bool isRightParenthesis();

    void setRightCurlyBrace();
    bool isRightCurlyBrace();

    void setLeftCurlyBrace();
    bool isLeftCurlyBrace();

    void isEndOfLineToken();
    bool isEndLinetoken();

    void setLineCount(int count);
    int getLineCount();


private:
    //attributes
    string _tokentype;
    string _token;

    bool isQuotedString;
    bool isString;
    bool isEndQuote;
    bool mySingleQuote;

    bool tokenEOF;

    bool improperSyntax;

    string syntaxMessage;


    bool _comma;
    bool _leftParen;
    bool _rightParen;
    bool _rightCurly;
    bool _leftCurly;

    bool endLineToken;

    int lineCount;



};


#endif //PROGRAMMINGASSIGNMENT1_TOKEN_H
