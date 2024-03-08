#include <iostream>
#include <fstream>
#include <string>

#include "CommentDeleter.cpp"

enum class EIdentifier
{
	state_0, //no_identifier,
	state_1, // Starts with a character from a - z and A - Z
	state_2, // finish with empty space
};

enum class EString
{
	state_0, // no string
	state_1, // Starts with " or '
	state_2, // Looping through the string
	state_3, // Ends with " or '
};

enum class ESymbol
{
	state_0, // no symbol
	state_1, // Starts with: =, |, & 
	state_2, // 
	state_3, // Ends with empty space
};

enum class EInteger
{
	state_0, // no integer,
	state_1, //	Starts with a digit
	state_2, // Looping through the integer
	state_3, // Ends with empty space or a symbol
};


std::string TOKENFILE;
EIdentifier IDENTIFIER;
EString ISSTRING;
ESymbol SYMBOL;
EInteger INTEGER;

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
		bool bCharacter = false;
		std::string extraSymbol = "";
		std::string tokenType = getTokenKind(std::string(1,c), &bCharacter);


		if (isdigit(c) || (c == '-' && isdigit(inFile->peek()))) // Check for integers
		{
			INTEGER = EInteger::state_1;
		}
		else if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) // Check for identifiers
		{
			IDENTIFIER = EIdentifier::state_1;
		}
		else if (bCharacter) // Check for symbols
		{
			SYMBOL = ESymbol::state_1;
		}
		if (c == '\'' || c == '"') // Check for strings
		{
			ISSTRING = EString::state_1;
		}


		// Check for new line
		if (c == '\n')
		{
			lineCounter++;
		}

		// ================================= Check for integers =================================
		if (INTEGER == EInteger::state_1)
		{
			std::string token;
			token += c;
			bool looping = true;
			while (looping)
			{
				char temp = inFile->peek();
				bool bValidToken = false;
				getTokenKind(std::string(1, temp), &bValidToken);
				if (temp == ' ' || temp =='\n' || bValidToken)
				{
					looping = false;
					break;
				}
				inFile->get(c);
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

		// ================================= Check for Identifiers =================================
		// From A - Z and a - z
		else if (IDENTIFIER == EIdentifier::state_1)
		{
			IDENTIFIER = EIdentifier::state_1;
			std::string token;

			token += c;
			bool looping = true;
			while (looping)
			{
				char temp = inFile->peek();

				bool bIdentifierBreak = false;
				getTokenKind(std::string(1, temp), &bIdentifierBreak);

				if (temp ==' \n' || temp == ' ' || bIdentifierBreak)
				{
					looping = false;
					break;
				}
				inFile->get(c);
				token += c;
			}
			IDENTIFIER = EIdentifier::state_2;
			PrintToken(outFile, token, "IDENTIFIER");
		}

		// ============= Check for symbols =============
		else if (SYMBOL == ESymbol::state_1)
		{
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
		}

		// =============  Check for strings ============= 
		if (ISSTRING == EString::state_1)
		{
			if (c == '\'')
			{
				ISSTRING = EString::state_1;
				std::string inString = "";
				while (inFile->get(c))
				{
					ISSTRING = EString::state_2;
					if (c == '\'')
					{
						ISSTRING = EString::state_3;
						break;
					}
					if (c == '\n')
					{
						lineCounter++;
					}
					inString += c;
				}
				bool bString = true;
				PrintToken(outFile, inString, "STRING"); // Print the string in the file
				PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
			}
			else if (c == '"')
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
					if (c == '\n')
					{
						lineCounter++;
					}

					inString += c;
				}
				bool bString = true;
				PrintToken(outFile, inString, "STRING"); // Print the string in the file
				PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
			}
		}
		
		// =============  Restart States =============
		IDENTIFIER = EIdentifier::state_0;
		ISSTRING = EString::state_0;
		SYMBOL = ESymbol::state_0;
		INTEGER = EInteger::state_0;
		
	}// End of while loop

	// if states are != 0, then there is an error
	if (IDENTIFIER != EIdentifier::state_0 || ISSTRING != EString::state_0 || SYMBOL != ESymbol::state_0 || INTEGER != EInteger::state_0)
	{
		outFile->close();
		outFile->open(TOKENFILE + "_tokenized.txt");
		// Print the all states that are != 0
		*outFile << "Identifier state: " << (int)IDENTIFIER << '\n'
				<< "String state: " << (int)ISSTRING << '\n'
				<< "Symbol state: " << (int)SYMBOL << '\n'
				<< "Integer state: " << (int)INTEGER << '\n'
				<< "Syntax error on line " << lineCounter << ": invalid token" << std::endl; // Print the line number

		return false;
	}

	return true;
}

int main()
{
	std::ifstream fileStream;
	std::ofstream outputStream;

	IDENTIFIER = EIdentifier::state_0;
	ISSTRING = EString::state_0;
	SYMBOL = ESymbol::state_0;
	INTEGER = EInteger::state_0;

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

		outputStream << "Token list: " << '\n' << '\n';
		GetCharacter(&fileStream, &outputStream);

		fileStream.close();
		outputStream.close();
	}
}