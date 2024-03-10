//
// Created by Spenc on 2/20/2024.
//

#include "token.h"

#include <utility>
#include <iostream>


string token::getTokenType() {
    return _tokentype;
}

string token::getToken() {
    return _token;
}

token::token() {
    _tokentype = "";
    _token = "";
    isQuotedString = false;
    isString = false;
    tokenEOF = false;
    mySingleQuote = false;

    _comma = false;
    _leftParen = false;
    _rightParen = false;
    _rightCurly = false;
    _leftCurly = false;
}

void token::setTokenType(const string& tokenType) {
    this->_tokentype = tokenType;
}

void token::setToken(const string& token) {
    this->_token = token;
}

void token::quotedString() {
    isQuotedString = true;
}

bool token::isQuotedSTR() {
    return isQuotedString;
}

void token::stringPrev() {
    isString = true;
}

bool token::isStringPrev() {
    return isString;
}

void token::printToken() {
    std::cout << "Token Type: " << _tokentype << std::endl << "Token: " << _token << std::endl << '\n';
}

void token::tokenIsEOF() {
    tokenEOF = true;
}

bool token::getTokenEOF() {
    return tokenEOF;
}

void token::endQuote() {
    isEndQuote = true;
}

bool token::isQuoteEnd() {
    return isEndQuote;
}

void token::singleQuote() {
    mySingleQuote = true;
}

bool token::isSingleQuote() {
    return mySingleQuote;
}

void token::incorrectSyntax() {
    improperSyntax = true;
}

bool token::syntaxError() {
    return improperSyntax;
}

void token::setSyntaxMessage(string msg) {
    syntaxMessage = msg;
}

string token::returnSyntaxmsg() {
    return syntaxMessage;
}

void token::setCommma() {
    _comma = true;
}

bool token::isComma() {
    return _comma;
}

void token::setLeftParenthesis() {
    _leftParen = true;
}

bool token::isLeftParenthesis() {
    return _leftParen;
}

void token::setRightParenthesis() {
    _rightParen = true;

}

bool token::isRightParenthesis() {
    return _rightParen;
}

void token::setRightCurlyBrace() {
    _rightCurly = true;
}

bool token::isRightCurlyBrace() {
    return _rightCurly;
}

void token::setLeftCurlyBrace() {
    _leftCurly = true;

}

bool token::isLeftCurlyBrace() {
    return _leftCurly;
}

void token::isEndOfLineToken() {
    endLineToken = true;
}

bool token::isEndLinetoken() {
    return endLineToken;
}

void token::setLineCount(int count) {
    lineCount = count;
}

int token::getLineCount() {
    return lineCount;
}

token::~token() = default;
