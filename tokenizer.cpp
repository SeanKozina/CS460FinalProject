#include <iostream>
#include <fstream>
#include <string>

#include "CommentDeleter.cpp"

enum class EIdentifier
{
	state_1, // Starts with a character from a - z and A - Z
	state_2, // finish with empty space
};

enum class EString
{
	state_1, // Starts with " or '
	state_2, // String
	state_3, // Ends with " or '
};

enum class ESymbol
{
	state_1, // Starts with: =, |, & 
	state_2, // 
	state_3, // Ends with empty space
};

std::string TOKENFILE;
EIdentifier IDENTIFIER;
EString ISSTRING;
ESymbol SYMBOL;

std::string getTokenKind(const std::string& input, bool* bToken)
{
    std::string character = "UNKNOWN"; 
    *bToken = true; 

    if (input == "(") character = "LEFT_PARENTHESIS";
    else if (input == ")") character = "RIGHT_PARENTHESIS";
    else if (input == "[") character = "LEFT_BRACKET";
    else if (input == "]") character = "RIGHT_BRACKET";
    else if (input == "{") character = "LEFT_BRACE";
    else if (input == "}") character = "RIGHT_BRACE";
    else if (input == "\"") character = "DOUBLE_QUOTE";
    else if (input == "'") character = "SINGLE_QUOTE";
    else if (input == ";") character = "SEMICOLON";
    else if (input == ",") character = "COMMA";
    else if (input == "=") character = "ASSIGNMENT";
    else if (input == "+") character = "PLUS";
    else if (input == "-") character = "MINUS";
    else if (input == "*") character = "ASTERISK";
    else if (input == "/") character = "DIVIDE";
    else if (input == "%") character = "MODULO";
    else if (input == "^") character = "CARET";
    else if (input == "<") character = "LESS_THAN";
    else if (input == ">") character = "GREATER_THAN";
    else if (input == ">=") character = "GREATER_THAN_OR_EQUAL";
    else if (input == "<=") character = "LESS_THAN_OR_EQUAL";
    else if (input == "|") character = "OR";
	else if (input == "||") character = "BOOLEAN_OR";
    else if (input == "&") character = "AND";
    else if (input == "&&") character = "BOOLEAN_AND";
    else if (input == "==") character = "EQUAL";
    else *bToken = false; 

    return character;
}

template <typename T, typename C>
void PrintToken(std::ofstream *outFile, T toPrint, C tokenType)
{
	*outFile << "Token type: " << tokenType << '\n';
	*outFile << "Token:      " << toPrint << '\n'
		  << '\n';
}

bool GetCharacter(std::ifstream *inFile, std::ofstream *outFile)
{
	char c;
	int lineCounter = 1;

	while (inFile->get(c))
	{
		// Check for new line
		if (c == '\n')
		{
			lineCounter++;
		}

		// ================================= Check for integers =================================
		if (isdigit(c) || (c == '-' && isdigit(inFile->peek())))
		{
			std::string token;
			token += c;
			bool looping = true;
			while (looping)
			{
				inFile->get(c);
				bool bValidToken = false;
				getTokenKind(std::string(1, c), &bValidToken);
				if (c == ' ' || c=='\n' || bValidToken)
				{
					if (c == '\n')
					{
						lineCounter++;
					}
					looping = false;
					break;
				}
				if (!isdigit(c) )
				{
					looping = false;
					outFile->close();
					outFile->open(TOKENFILE + "_tokenized.txt");
					*outFile << "Syntax error on line " << lineCounter << ": invalid integer" << std::endl;
					return false;
				}
				token += c;
			}
			PrintToken(outFile, token, "INTEGER");
		}

		// =================================Check for Identifiers =================================
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
				getTokenKind(std::string(1, c), &bIdentifierBreak);

				if (c ==' \n' || c == ' ' || bIdentifierBreak)
				{
					if (c == '\n')
					{
						lineCounter++;
					}
					looping = false;
					break;
				}
				token += c;
			}
			IDENTIFIER = EIdentifier::state_2;
			PrintToken(outFile, token, "IDENTIFIER");
		}

		// ============= Check for symbols ============= 
		bool bCharacter = false;
		std::string extraSymbol = "";
		std::string tokenType = getTokenKind(std::string(1,c), &bCharacter);
		if (bCharacter && (inFile->peek() == '=' || inFile->peek() == '&' || inFile->peek() == '|'))
		{
			SYMBOL = ESymbol::state_1;
			extraSymbol += c;
			inFile->get(c);
			extraSymbol += c;
		}
		else
		{
			extraSymbol += c;
		}
		tokenType = getTokenKind(extraSymbol, &bCharacter);
		if (bCharacter)
		{
			PrintToken(outFile, extraSymbol, tokenType);
		}


		// =============  Check for strings ============= 
		if (c == '\'')
		{
			ISSTRING = EString::state_1;
			std::string inString = "";
			while (inFile->get(c))
			{
				ISSTRING = EString::state_2;
				if (c == '\n')
				{
					lineCounter++;
				}
				if (c == '\'')
				{
					ISSTRING = EString::state_3;
					break;
				}
				inString += c;
			}
			bool bString = true;
			PrintToken(outFile, inString, "STRING"); // Print the string in the file
			PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
		}
		if (c == '"')
		{
			ISSTRING = EString::state_1;
			std::string inString = "";
			while (inFile->get(c))
			{
				ISSTRING = EString::state_2;
				if (c == '\n')
				{
					lineCounter++;
				}
				if (c == '"')
				{
					ISSTRING = EString::state_3;
					break;
				}
				inString += c;
			}
			bool bString = true;
			PrintToken(outFile, inString, "STRING"); // Print the string in the file
			PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
		}
	}// End of while loop


	// if (IDENTIFIER != EIdentifier::state_2 || ISSTRING != EString::state_3)
	// {
	// 	return true;
	// }
	

	return true;
}

int main()
{
	std::ifstream fileStream;
	std::ofstream outputStream;

	for (int i = 1; i < 7; i++)
	{
		TOKENFILE = "test" + std::to_string(i) + ".c";

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

		outputStream << "Token list: " << '\n'
					 << '\n';
		GetCharacter(&fileStream, &outputStream);

		fileStream.close();
		outputStream.close();
	}
}