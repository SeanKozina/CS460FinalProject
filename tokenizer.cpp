//
// Created by Spenc on 2/21/2024.
//

#include <iostream>
#include "tokenizer.h"


tokenizer::tokenizer(const string& inputFile) {

    _inputStream.open(inputFile);

    fileName = inputFile;

    endFile = false;

    lineCount = 1;

}

token tokenizer::getToken() {

    char c;

    string character = "0";


    //state 0 = initial state, state 1 = identifier, state 2 = char of interest

    int state = 0;


    token myToken;

    if(_inputStream.eof()){
        myToken.tokenIsEOF();
        return myToken;
    }


    //int lineCount = 0;

    string tokenName;
    string tokenType;

    bool endLoop = false;

    while(!endLoop) {
        _inputStream.get(c);

        if(c == '\n'){
            lineCount++;
        }


        if(checkCharInterest(c)){
            state = 2;
        }else if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
            state = 1;
        }
        else if(c == '"' || c == '\''){
            state = 3;
        }
        else if(isdigit(c)){
            state = 4;
        }
        else if(c == '-'){
            state = 5;
        }
        else if( c == '\\'){
            state = 6;
        }
        else if(c == ','){
            state = 7;
        }
        else if(c == '&'){
            state = 8;
        }
        else if(c == '|'){
            state = 9;
        }
        else if(c == '%'){
            state = 10;
        }



        if (state == 1) {

            tokenName += c;

            if(prevToken.isQuotedSTR()){
                while(_inputStream.get(c)){
                    if(c == '"'){
                        _inputStream.putback(c);
                        endLoop = true;
                        break;
                    }

                    tokenName += c;

                }

                tokenType = "STRING";
                myToken.setToken(tokenName);
                myToken.setTokenType(tokenType);
                myToken.stringPrev();
                prevToken = myToken;

                break;

            }

            else if(prevToken.isSingleQuote()){

                while(_inputStream.get(c)){
                    if(c == '\''){
                        _inputStream.putback(c);
                        endLoop = true;
                        break;
                    }

                    tokenName += c;

                }

                tokenType = "STRING";
                myToken.setToken(tokenName);
                myToken.setTokenType(tokenType);
                myToken.stringPrev();
                prevToken = myToken;

                break;
            }



            while (_inputStream.get(c)) {
                if (c == ' ') {
                    endLoop = true;
                    break;

                }
                else if(checkCharInterest(c) || c == '"' || c == ',' || c== '\''){
                    _inputStream.putback(c);
                    endLoop = true;
                    break;
                }

                tokenName += c;
            }

            tokenType = "IDENTIFIER";

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);

        } else if(state == 2){


            if(c == '=' && _inputStream.peek() == '='){
                _inputStream.get(c);
                tokenType = "EQUAL";
                tokenName = "==";

                myToken.setTokenType(tokenType);
                myToken.setToken(tokenName);
                endLoop = true;
                break;

            }
            else if(c == '<' && _inputStream.peek() == '='){
                _inputStream.get(c);
                tokenType = "LESS_THAN_OR_EQUAL_TO";
                tokenName = "<=";

                myToken.setTokenType(tokenType);
                myToken.setToken(tokenName);
                endLoop = true;
                break;
            }
            else if(c == '>' && _inputStream.peek() == '='){
                _inputStream.get(c);
                tokenType = "GREATER_THAN_OR_EQUAL_TO";
                tokenName = ">=";

                myToken.setTokenType(tokenType);
                myToken.setToken(tokenName);
                endLoop = true;
                break;
            }

//            if(c == ';'){
//                myToken.setCommma();
//            }else if(c == '('){
//                myToken.setLeftParenthesis();
//            }
//            else if(c == ')'){
//                myToken.setRightParenthesis();
//            }
//            else if(c == '{'){
//                myToken.setLeftCurlyBrace();
//            }
//            else if(c == '{'){
//                myToken.setRightCurlyBrace();
//            }




            tokenType = charOfInterest(c);
            tokenName = c;

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
            endLoop = true;
        }
        else if(state == 3){

            tokenType = charOfInterest(c);
            tokenName = c;

            if(!prevToken.isStringPrev()){

                if(c == '"') {
                    myToken.quotedString();
                }
                else if(c == '\''){
                    myToken.singleQuote();

                }

            }






            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);

            prevToken = myToken;

            endLoop = true;

        }
        else if(state == 4){

            if(prevToken.isSingleQuote()){

                tokenName = c;

                tokenType = "STRING";


                myToken.setTokenType(tokenType);
                myToken.setToken(tokenName);

                myToken.stringPrev();

                prevToken = myToken;

                endLoop = true;

                break;

            }

            if(isalpha(_inputStream.peek())){


                myToken.incorrectSyntax();


                string errorMsg = "Syntax error on line " + to_string(lineCount) + ": invalid integer";

                myToken.setSyntaxMessage(errorMsg);

                endLoop = true;
                break;

            }



            tokenType = "INTEGER";

            tokenName = c;

            while(_inputStream.get(c)){
                if(isdigit(c)){
                    tokenName += c;
                }
                else{
                    _inputStream.putback(c);
                    break;
                }
            }

            endLoop = true;

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
        }

        else if(state == 5){

            tokenName = c;


            _inputStream.get(c);

            if(isdigit(c)){
                tokenType = "INTEGER";

                tokenName += c;


                while(_inputStream.get(c)){
                    if(isdigit(c)){
                        tokenName += c;
                    }
                    else{
                        _inputStream.putback(c);
                        break;
                    }
                }
            }
            else if(c == ' '){
                tokenType = "MINUS";
            }
            else{
                tokenType = charOfInterest(c);
            }

            endLoop = true;

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
            prevToken = myToken;

        }
        else if(state == 6){

            tokenName = c;

            //_inputStream.get(c);

            if(prevToken.isQuotedSTR()){
                tokenType = "STRING";
                while(_inputStream.get(c)){
                    if(c == '"'){
                        _inputStream.putback(c);
                        endLoop = true;
                        myToken.stringPrev();
                        break;
                    }

                    tokenName += c;
                }
            }
            else if(prevToken.isSingleQuote()){
                tokenType = "STRING";

                if(_inputStream.peek() == '0' || _inputStream.peek() == 'n'){
                    _inputStream.get(c);
                    tokenName+=c;
                }


                myToken.stringPrev();



            }


            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
            prevToken = myToken;

            endLoop = true;


        }
        else if(state == 7){

            tokenName = c;

            //_inputStream.get(c);

            if(prevToken.isQuotedSTR()){
                tokenType = "STRING";
                while(_inputStream.get(c)){
                    if(c == '"'){
                        _inputStream.putback(c);
                        endLoop = true;
                        break;
                    }

                    tokenName += c;
                }

                myToken.stringPrev();
            }
            else{
                tokenType = charOfInterest(c);
            }


            endLoop = true;

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
            prevToken = myToken;

        }
        else if(state == 8){

            tokenName = c;

            _inputStream.get(c);

            if(c == '&'){

                tokenName += c;
                tokenType = "BOOLEAN_AND";


            }
            else{
                tokenType = "AND";
                _inputStream.putback(c);
            }


            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);

            endLoop = true;

        }
        else if(state == 9){

            tokenName = c;
            //if(_inputStream.peek() == '|'){
                tokenName += c;
                tokenType = "BOOLEAN_OR";
                myToken.setTokenType(tokenType);
                myToken.setToken(tokenName);
                endLoop = true;
            //}


        }
        else if(state == 10){

            tokenName = c;
            tokenType = "MODULO";

            if(prevToken.isQuotedSTR()){
                while(_inputStream.get(c)){
                    if(c == '"'){
                        _inputStream.putback(c);
                        //endLoop = true;
                        break;
                    }

                    tokenName += c;
                }

                tokenType = "STRING";
                myToken.stringPrev();

            }

            myToken.setTokenType(tokenType);
            myToken.setToken(tokenName);
            endLoop = true;
            prevToken = myToken;

        }

        if(c == '\000'){
            endFile = true;
            endLoop = true;
        }

        if(_inputStream.eof()){
            endLoop = true;
            endFile = true;
        }

    }


    myToken.setLineCount(lineCount);


    return myToken;


}

string tokenizer::charOfInterest(char c) {

    string character = "0";

    switch (c) {
        case '(': character = "LEFT_PARENTHESIS"; break;
        case ')': character = "RIGHT_PARENTHESIS"; break;
        case '[': character = "LEFT_BRACKET"; break;
        case ']': character = "RIGHT_BRACKET"; break;
        case '{': character = "LEFT_BRACE"; break;
        case '}': character = "RIGHT_BRACE"; break;
        case '"': character = "DOUBLE_QUOTE"; break;
        case '\'':character = "SINGLE_QUOTE"; break;
        case ';': character = "SEMICOLON"; break;
        case ',': character = "COMMA"; break;
        case '=': character = "ASSIGNMENT_OPERATOR"; break;
        case '+': character = "PLUS"; break;
        case '-': character = "MINUS"; break;
        case '*': character = "ASTERISK"; break;
        case '/': character = "DIVIDE"; break;
        case '%': character = "MODULO"; break;
        case '^': character = "CARET"; break;
        case '<': character = "LESS_THAN"; break;
        case '>': character = "GREATER_THAN"; break;
        default: character = "0"; break;
            break;
    }

    return character;
}

bool tokenizer::checkCharInterest(char c) {

    if (c =='(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}'
    || c == ';' || c == '=' || c == '+'  || c == '*' || c == '/'||
     c == '^' || c == '<' || c == '>'){
        return true;
    }
    else{
        return false;
    }

}

bool tokenizer::isEOF() {
    if(_inputStream.eof()){
        return true;
    }
    return false;
}

bool tokenizer::endOfFile() {

    return endFile;

}

bool tokenizer::isPrevTokenEndline() {
    return prevToken.isEndLinetoken();
}



