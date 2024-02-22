
#include <iostream>
#include <fstream>
#include <string>


enum class EIdentifier
{
	state_1, // Starts with a character from a - z and A - Z
	state_2, // finish with empty space
};

enum class EString
{
	state_1, // Starts with "
	state_2, // String
	state_3, // Ends with "
};


std::string getTokenKind(char input, bool* bToken = new bool(0)){
	std::string character = "0";
	switch (input) {
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
		case '=': character = "ASSIGNMENT"; break;
		case '+': character = "PLUS"; break;
		case '-': character = "MINUS"; break;
		case '*': character = "ASTERISK"; break;
		case '/': character = "DIVIDE"; break;
		case '%': character = "MODULO"; break;
		case '^': character = "CARET"; break;
		case '<': character = "LESS_THAN"; break;
		case '>': character = "GREATER_THAN"; break;
		case '>=': character = "GREATER_THAN_OR_EQUAL"; break;
		case '<=': character = "LESS_THAN_OR_EQUAL"; break;
		case '&&': character = "BOOLEAN_AND"; break;
		case '==' : character = "EQUAL"; break;
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
	*file << "Token:      " << toPrint << '\n' << '\n';
}

EIdentifier IDENTIFIER;
EString ISSTRING;


void GetCharacter(std::ifstream* inFile, std::ofstream* outFile)
{
	char c;
	while (inFile->get(c))
	{
		// Check for integers
		if (isdigit(c) || (c == '-' && isdigit(inFile->peek())))
		{
			std::string token;
			token += c;
			bool looping = true;
			while (looping)
			{
				inFile->get(c);
				if (!isdigit(c))
				{
					looping = false;
					break;
				}
				token += c;
			}
			PrintToken(outFile, token, "INTEGER");
		}
		
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
			PrintToken(outFile, token, "IDENTIFIER");
		}

		// Check for symbols
		bool bCharacter = false;
		std::string tokenType = getTokenKind(c, &bCharacter);
		if (bCharacter)
		{
			PrintToken(outFile, c, tokenType);
		}

		// Check for strings
		if (c == '"')
		{
			ISSTRING = EString::state_1;
			std::string inString = "";
			while (inFile->get(c))
			{
				ISSTRING = EString::state_2;
				if (c == '"')
				{
					ISSTRING = EString::state_3;
					break;
				}
				inString += c;
			}
			PrintToken(outFile, inString, "STRING"); // Print the string in the file
			PrintToken(outFile, c, getTokenKind(c)); // Print the second " in the file

		}

		

	}

	return; 
}


int main()
{
	std::ifstream fileStream;
	std::ofstream outputStream;

	std::string fileName;
	std::string outputFile = "myoutput2.txt";

	std::cout << "Enter name of file" << std::endl;
	//std::cin >> fileName;

	fileName = "test2.c";


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


	outputStream << "Token list: " << '\n' << '\n';
	GetCharacter(&fileStream, &outputStream);

}