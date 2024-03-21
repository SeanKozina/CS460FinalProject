
#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"


std::string tokenizer::getTokenKind(const std::string& input, bool* bToken)
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
    //else if (input == "\"") character = "EQUAL";
    else *bToken = false; 

    return character;
}

template <typename T, typename C>
void tokenizer::PrintToken(std::ofstream *outFile, T toPrint, C tokenType)
{
	*outFile << "Token type: " << tokenType << '\n';
	*outFile << "Token:      " << toPrint << '\n'
		  << '\n';
}

bool tokenizer::GetCharacter(std::ifstream *inFile, std::ofstream *outFile, std::string TOKENFILE)
{
	char c;
	int lineCounter = 1;

	EIdentifier IDENTIFIER = EIdentifier::state_0;
	EString ISSTRING = EString::state_0;
	ESymbol SYMBOL = ESymbol::state_0;
	EInteger INTEGER = EInteger::state_0;

	while (inFile->get(c))
	{
		// =============  Restart States =============
		IDENTIFIER = EIdentifier::state_0;
		ISSTRING = EString::state_0;
		SYMBOL = ESymbol::state_0;
		INTEGER = EInteger::state_0;

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
		if (c == '\'' || c == '\"') // Check for strings
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
			if (c == '[' && inFile->peek() == '-')
			{
				outFile->close();
				outFile->open(TOKENFILE + "_tokenized.txt");
				*outFile << "Syntax error on line " << lineCounter << " array size must be declared as a positive integer" << std::endl;
				return false;

			}
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
				int offsetLine = 0;
				while (inFile->get(c))
				{
					ISSTRING = EString::state_2;
					if (c == '\\' && std::string(1, inFile->peek()) == "\'")
					{
						inString += "\'";
						inFile->get(c);
					}

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
				if (ISSTRING == EString::state_3)
				{
					bool bString = true;
					PrintToken(outFile, inString, "STRING"); // Print the string in the file
					PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
				}
				else {
					outFile->close();
					outFile->open(TOKENFILE + "_tokenized.txt");
					*outFile << "Syntax error on line " << lineCounter - offsetLine << " Missing quoute" << std::endl;
					return false;
				}
			}
			else if (c == '"')
			{
				ISSTRING = EString::state_1;
				std::string inString = "";
				int offsetLine = 0;
				while (inFile->get(c))
				{
					ISSTRING = EString::state_2;
					if (c == '\\' && std::string(1, inFile->peek()) == "\"")
					{
						inString += "\"";
						inFile->get(c);
					}
					else if (c == '"')
					{
						ISSTRING = EString::state_3;
						break;
					}
					else if (c == '\n')
					{
						lineCounter++;
						offsetLine++;
					}
					else {
						inString += c;

					}

				}
				if (ISSTRING == EString::state_3)
				{
					bool bString = true;
					PrintToken(outFile, inString, "STRING"); // Print the string in the file
					PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
				}
				else {
					outFile->close();
					outFile->open(TOKENFILE + "_tokenized.txt");
					*outFile << "Syntax error on line " << lineCounter - offsetLine << " Missing quoute" << std::endl;
					return false;
				}

			}
		}
		
		
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

std::string tokenizer::GetCharacterForConcreteSyntax(std::ifstream *inFile, std::ofstream *outFile, int& lineCounter)
{
	char c;

	std::string TOKENFILE;
	EIdentifier IDENTIFIER = EIdentifier::state_0;
	EString ISSTRING = EString::state_0;
	ESymbol SYMBOL = ESymbol::state_0;
	EInteger INTEGER = EInteger::state_0;

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
					return "/error/";
				}
				token += c;
			}
			return token;
			//PrintToken(outFile, token, "INTEGER");
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
			return token;
			//PrintToken(outFile, token, "IDENTIFIER");
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
				return extraSymbol;
				//PrintToken(outFile, extraSymbol, tokenType);
			}
		}

		// =============  Check for strings ============= 
		if (ISSTRING == EString::state_1)
		{
			if (c == '\'') // check for '
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
				return inString;
				PrintToken(outFile, inString, "STRING"); // Print the string in the file
				PrintToken(outFile, c, getTokenKind(std::string(1, c), &bString)); // Print the second " in the file
			}
			else if (c == '"') // check for "
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
				return inString;
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

		return "/error/";
	}

	return "/end/";
}
