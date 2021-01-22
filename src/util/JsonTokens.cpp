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
            cursor = getPunctuation(cursor, c);
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

int JsonTokens::getPunctuation(int start_index, const char& c)
{
    Token token;
    if (c == open_brace)
        token = Token(TokenKind::OpenBrace, "{");
    else if (c == close_brace)
        token = Token(TokenKind::CloseBrace, "}");
    else if (c == open_square)
        token = Token(TokenKind::OpenSquare, "[");
    else if (c == close_square)
        token = Token(TokenKind::CloseSquare, "]");
    else if (c == comma)
        token = Token(TokenKind::Comma, ",");
    else if (c == colon)
        token = Token(TokenKind::Colon, ":");
    else if (c == quotation_mark)
        token = Token(TokenKind::QuotationMark, "\"");

    tokens.emplace_back(token);
    return start_index + 1;
}

int JsonTokens::getBool(int start_index, string& str)
{
	int end_index = start_index;
	if (str[start_index] == 'f') {
		tokens.emplace_back(TokenKind::Boolean, "false");
		end_index = start_index + 5;
	}
	else if (str[start_index] == 't') {
		tokens.emplace_back(TokenKind::Boolean, "true");
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
	tokens.emplace_back(TokenKind::String, s);
	tokens.emplace_back(TokenKind::QuotationMark, "\"");
	return end_index + 1;
}

int JsonTokens::getNumber(int start_index, string& str) 
{
	int end_index = start_index;
	while (isNumber(str[end_index]))
		end_index++;

	tokens.emplace_back(TokenKind::Number, str.substr(start_index, end_index - start_index));
	return end_index;
}
