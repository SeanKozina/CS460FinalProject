
#include <iostream>
#include <fstream>
#include <string>


enum class EIdentifier
{
	state_1, // Starts with a character from a - z and A - Z
	state_2, // finish with empty space
};


std::string getTokenKind(char input, bool* bToken) {
	std::string character = "0";
	switch (input) {
	case '(': character = "L_PAREN"; break;
		case ')': character = "R_PAREN"; break;
		case '[': character = "L_BRACKET"; break;
		case ']': character = "R_BRACKET"; break;
		case '{': character = "L_BRACE"; break;
		case '}': character = "R_BRACE"; break;
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
		case '<': character = "LT"; break;
		case '>': character = "GT"; break;
		default: character = "0"; break;
			break;
	}
	if (character == "0")
	{
		*bToken = false;
	}
	else
	{
		*bToken = true;
	}

	return character;
}

template <typename T>
void PrintToken(std::ofstream* file, T toPrint, std::string tokenType)
{
	*file << "Token type: " << tokenType << '\n';
	*file << "Token: " << toPrint << '\n' << '\n';
}

EIdentifier IDENTIFIER;


void GetCharacter(std::ifstream* inFile, std::ofstream* outFile)
{
	char c;
	while (inFile->get(c))
	{
		// Check for Identifiers
		// From A - Z and a - z
		if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		{
			IDENTIFIER = EIdentifier::state_1;
			std::string token;

			token += c;
			bool looping = true;
			while (looping)
			{
				inFile->get(c);
				bool bIdentifierBreak = false;
				getTokenKind(c, &bIdentifierBreak);

				if (c == 32 || bIdentifierBreak)
				{
					looping = false;
					break;
				}
				token += c;

			}
			IDENTIFIER = EIdentifier::state_2;
			std::cout << token;
			PrintToken(outFile, token, "IDENTIFIER");
		}

		// Check for symbols
		bool bCharacter = false;
		std::string tokenType = getTokenKind(c, &bCharacter);
		if (bCharacter)
		{
			PrintToken(outFile, c, tokenType);
		}


	}

	return; 
}


int main()
{
	std::ifstream fileStream;
	std::ofstream outputStream;

	std::string fileName;
	std::string outputFile = "myoutput.txt";

	std::cout << "Enter name of file" << std::endl;
	//std::cin >> fileName;

	fileName = "test1.c";


	//open file
	fileStream.open(fileName);
	outputStream.open(outputFile);

	if (!fileStream.is_open())
	{
		std::cout << "File not found";
		return 1; // Changed to indicate an error
	}

	if (!outputStream.is_open())
	{
		std::cout << "Failed to open output file";
		return 1; // Changed to indicate an error
	}


	outputStream << "Token List: " << '\n' << '\n';
	GetCharacter(&fileStream, &outputStream);

}