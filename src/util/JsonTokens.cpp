#include "JsonTokens.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Log.h"
#include "game-manager/FilePath.h"

using namespace std;
/**
 * Turns a json file into vector of tokens
 */
JsonTokens::JsonTokens(const string& file_name)
{
	// turn file into one long string
	stringstream file_string;

	string file_path = FilePath::json_path + file_name + ".json";
	fstream file(file_path);

	if (!file.is_open())
		Error::file("json", file_path);

	string str;
	while (getline(file, str))
		file_string << str;
	file.close();

	// turn string into tokens
	str = file_string.str();

	string token;
	int cursor = 0;
	while(cursor < str.length()) {
		char c = str[cursor];
		if (isWhiteSpace(c))
			cursor++;
		else if (isPunctuation(c)) {
			tokens.emplace_back(1, c);
			cursor++;
			// string
			if (isQuotation(c))
				cursor = getString(cursor, str);
		}
		else if (isNumber(c))
			cursor = getNumber(cursor, str);
		else
			cursor = getBool(cursor, str);
	}
}

int JsonTokens::getBool(int start_index, string& str)
{
	int end_index = start_index;
	if (str[start_index] == 'f') {
		tokens.emplace_back("false");
		end_index = start_index + 5;
	}
	else if (str[start_index] == 't') {
		tokens.emplace_back("true");
		end_index = start_index + 4;
	}
	return end_index;
}

int JsonTokens::getString(int start_index, string& str)
{
	int end_index = start_index;
	while (!isQuotation(str[end_index]))
		end_index++;

	string s = str.substr(start_index, end_index - start_index);
	tokens.emplace_back(s);
	tokens.emplace_back(1, quotation_mark);
	return end_index + 1;
}

int JsonTokens::getNumber(int start_index, string& str) 
{
	int end_index = start_index;
	while (isNumber(str[end_index]))
		end_index++;

	tokens.emplace_back(str.substr(start_index, end_index - start_index));
	return end_index;
}
