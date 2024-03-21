#pragma once

#include <string>

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

class tokenizer {
public:
	std::string getTokenKind(const std::string& input, bool* bToken);

	template <typename T, typename C>
	void PrintToken(std::ofstream* outFile, T toPrint, C tokenType);
	bool GetCharacter(std::ifstream* inFile, std::ofstream* outFile, std::string FILENAME);
	std::string GetCharacterForConcreteSyntax(std::ifstream* inFile, std::ofstream* outFile, int& lineC);

};