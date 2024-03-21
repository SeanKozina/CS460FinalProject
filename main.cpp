#include <iostream>
#include <fstream>
#include <string>

#include "CommentDeleter.cpp"
#include "Tokenizer.h"
#include "concreteSyntaxTree.h"

int main()
{
	std::ifstream fileStream;
	std::ofstream outputStream;

	// 1 to 10
	for (int i = 1; i <= 10; i++)
	{
		std::string TOKENFILE = "test" + std::to_string(i) + ".c";

		// First Remove Comments:
		if (!RemoveComments(TOKENFILE))
		{
			std::cout << "Failed to remove comments";
			return 1; // Changed to indicate an error
		}

		// open file
		fileStream.open(TOKENFILE + "_no_comments.c");

		if (!fileStream.is_open())
		{
			std::cout << "File not found";
			return 1; // Changed to indicate an error
		}

		std::string outputFile = TOKENFILE + "_tokenized.txt";
		outputStream.open(outputFile);

		if (!outputStream.is_open())
		{
			std::cout << "Failed to open output file";
			return 1; // Changed to indicate an error
		}
        
        // project 2
		outputStream << "Token list: " << '\n' << '\n';
		tokenizer tokenizerV;
		bool pass = tokenizerV.GetCharacter(&fileStream, &outputStream, TOKENFILE);
		
		fileStream.close();
		outputStream.close();

		
        // project 3
		if (pass)
		{
			fileStream.open(TOKENFILE + "_no_comments.c");
			outputFile = TOKENFILE + "_SyntaxTree.txt";
			outputStream.open(outputFile);

			concreteSyntaxTree* syntaxTree = new concreteSyntaxTree;
			syntaxTree->createConcreteSyntaxTree(&fileStream, &outputStream, TOKENFILE);

			fileStream.close();
			outputStream.close();
		}

	}// end for loop


} // end main