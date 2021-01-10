#include "JsonParser.h"

#include <iostream>

#include "JsonTokens.h"
#include "Log.h"
#include "JsonValue.h"

using namespace std;

int JsonParser::current = 0;
vector<string> JsonParser::tokens;

regex JsonParser::curly_brace_open = regex("\\{");
regex JsonParser::curly_brace_close = regex("\\}");
regex JsonParser::square_brace_open = regex("\\[");
regex JsonParser::square_brace_close = regex("\\]");
regex JsonParser::number = regex("[+-]?([0-9]*[.])?[0-9]+");
regex JsonParser::quotation_mark = regex("\"");
regex JsonParser::comma = regex(",");
regex JsonParser::colon = regex(":");
regex JsonParser::boolean = regex("true|false");

JsonValue JsonParser::parse(const std::string& file_name)
{
	JsonTokens tokenizer = JsonTokens(file_name);

	tokens = tokenizer.tokens;
	current = 0;
	//for (auto t: tokens)
		//cout << t << ' ';
	//cout << endl;

	if (isMatch(curly_brace_open))
		return { parseObject() };
	else if (isMatch(square_brace_open))
		return { parseArray() };
	else {
		Log::s(getCurrent());
		Error::exit("No valid object/array");
		return {};
	}
}

String JsonParser::parseKey()
{
	require(quotation_mark);
	auto str = getCurrent();
	require(quotation_mark);
	require(colon);
	return str;
}

Object JsonParser::parseObject()
{
	require(curly_brace_open);
	Object obj;
	bool new_value = true;
	while (new_value) {
		String key = parseKey();
		JsonValue value = parseValue();
		obj.insert(key, value);
		new_value = isMatch(comma);
		if (new_value)
			current++;
	}
	require(curly_brace_close);
	return obj;
}

JsonValue JsonParser::parseValue() 
{
	if (isMatch(square_brace_open))
		return { parseArray() };

	else if (isMatch(curly_brace_open))
		return { parseObject() };

	else if (isMatch(number))
		return { parseNumber() };

	else if (isMatch(boolean))
		return { parseBoolean() };

	else if (isMatch(quotation_mark))
		return { parseString() };

	else {
		Error::exit("No valid value");
		return {};
	}
}

Bool JsonParser::parseBoolean() 
{
	bool b = false;
	if (getCurrent().compare("true") == 0)
		b = true;
	return b;
}

Number JsonParser::parseNumber()
{
	float num = stof(getCurrent());
	return num;
}

String JsonParser::parseString()
{
	require(quotation_mark);
	std::string str = getCurrent();
	require(quotation_mark);
	return str;
}

Array JsonParser::parseArray()
{
	Array arr;
	require(square_brace_open);
	bool new_value = true;
	while (new_value) {
		arr.insert(parseValue());
		new_value = isMatch(comma);
		if (new_value)
			current++;
	}
	require(square_brace_close);
	return arr;
}

const std::string& JsonParser::getCurrent() 
{
	current++;
	return tokens[current - 1];
}

bool JsonParser::isMatch(std::regex pattern)
{
	bool match = regex_match(tokens[current], pattern);
	return match;
}

void JsonParser::require(std::regex pattern)
{
	if (!isMatch(pattern))
		Error::exit("match error: " + tokens[current]);
	current++;
}
