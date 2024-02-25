/* Name: Spencer, Isauro, Daniel, Sean, Ethan
 * Date: 2/4/24
 * Description: A program in C++ that will identify and remove comments from C and C++ files.
 *
 * */

#include <iostream>
#include "fstream"
#include "string"

using namespace std;



bool RemoveComments(string fileName){

    //enumerated class for states
    enum state{
        state0, //initial state
        state1, //found inital forward slash
        state2a, //second forward slash for c++ style comments
        state2b, //found star for c style comments
        state3a, //read in characters
        state3b, // read in characters
        state4a, //found newline char to end c++ comment
        state4b, //found star followed by ascii characters
        state5a //found a star followed by a forward slash to end c style comment
    };


    //stream for reading files
    ifstream fileStream;

    //stream for output to files
    ofstream outputStream;

    //filename variable to output
    string outputFile = fileName + "_no_comments.c";



    //open file
    fileStream.open(fileName);
    outputStream.open(outputFile);

    char c;
    //counter for line # if we have a syntax error
    int lineCount = 1;

    state curState = state0; // initialize our currrent state to state 0 to begin


    bool startTag = false; // /*
    bool endTag = false; // */

    int lineCountForError = 0;

    while(fileStream.get(c)) {

        if(c == '\n'){
            lineCount++;
        }

        switch (curState) {
            case state0:
                if (c == '/') {
                    if( fileStream.peek() == '/' || fileStream.peek() == '*'){
                        curState = state1;
                        break;
                    }
                    outputStream.put(c);


                } else if(c == '"') {
                    if (fileStream.peek() == '/') {
                        outputStream.put(c);
                        fileStream.get(c);
                        outputStream.put(c);

                       if(fileStream.peek() == '/' || fileStream.peek() == '*'){

                           while(fileStream.get(c)){

                               if(c == '"'){
                                   outputStream.put(c);
                                   break;
                               }

                               outputStream.put(c);
                           }

                       }

                    } else {
                        outputStream.put(c);
                    }
                } else if(c == '*' && fileStream.peek() == '/'){
                    endTag = true;
                    lineCountForError = lineCount;

                    cerr << "ERROR: Program contains C-style, unterminated comment on line " << lineCountForError
                         << endl;

                    exit(-2);


                } else {

                    outputStream.put(c); // if its not started by a / then it is lines of code we want to keep
                }

                break;

            case state1:
                if (c == '/') {
                    curState = state2a;

                } else if (c == '*') {
                    startTag = true;
                    curState = state2b;
                    outputStream.put(' ');
                    outputStream.put(' ');
                    lineCountForError = lineCount;

                }
                break;

            case state2a:
                if (c != '\n') {
                    curState = state3a;

                } else {
                    curState = state4a;
                }

                break;

            case state2b:


                if (c == '*' && fileStream.peek() == '/') {
                    curState = state4b;

                    outputStream.put(' ');
                    break;
                }

                curState = state3b;

                if(c == '\n'){
                    outputStream.put(c);
                    break;
                }

                outputStream.put(' ');

                break;

            case state3a:

                if(c == '\n'){
                    curState = state4a;
                    outputStream.put(c);
                    break;
                }

                while (fileStream.get(c)) {
                    if(fileStream.peek() == '\n'){
                        curState = state4a;
                        break;
                    }
                }
                break;

            case state3b:
                if (c == '*' && fileStream.peek() == '/') {
                    curState = state4b;
                    outputStream.put(' ');
                    break;

                }
                if(c == '\n'){
                    outputStream.put(c);
                    break;
                }
                outputStream.put(' ');

                curState = state3b;

                break;


            case state4a:
                //end of c++ style comment
                //outputStream.put('\n');
                outputStream.put(c);
                curState = state0; // set back to initial state to find more comments
                break;


            case state4b:
                if (c == '/') {
                    endTag = true;
                    outputStream.put(' ');
                    curState = state5a;
                } else {
                    curState = state3b;
                    outputStream.put(' ');
                }
                break;

            case state5a:
                //end of c style comment
                curState = state0; // set back to initial state to find more comments

                outputStream.put(c);

                startTag = false;
                endTag = false;

                break;

        }
    }

    if(startTag != endTag){
        cerr << "ERROR: Program contains C-style, unterminated comment on line " << lineCountForError
                             << endl;

        return false;
    }
    return true;
}




