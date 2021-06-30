#include "JsonParser.h"

#include <iostream>

#include "JsonTokens.h"
#include "JsonValue.h"
#include "Log.h"

using namespace std;

int JsonParser::current = 0;
vector<Token> JsonParser::tokens;

JsonValue JsonParser::parse(const std::string& file_name)
{
	JsonTokens tokenizer = JsonTokens(file_name);

	tokens = tokenizer.tokens;
	current = 0;

	if (isMatch(TokenKind::OpenBrace))
		return { parseObject() };
	else if (isMatch(TokenKind::OpenSquare))
		return { parseArray() };
	else {
		Log::s(getCurrent());
		Error::exit("No valid object/array");
		return {};
	}
}

String JsonParser::parseKey()
{
	require(TokenKind::QuotationMark);
	auto str = getCurrent();
	require(TokenKind::QuotationMark);
	require(TokenKind::Colon);
	return str;
}

Object JsonParser::parseObject()
{
	require(TokenKind::OpenBrace);
	Object obj;
	bool new_value = true;
	while (new_value) {
		String key = parseKey();
		JsonValue value = parseValue();
		obj.insert(key, value);
		new_value = isMatch(TokenKind::Comma);
		if (new_value)
			current++;
	}
	require(TokenKind::CloseBrace);
	return obj;
}

JsonValue JsonParser::parseValue() 
{
	if (isMatch(TokenKind::OpenSquare))
		return { parseArray() };

	else if (isMatch(TokenKind::OpenBrace))
		return { parseObject() };

	else if (isMatch(TokenKind::Number))
		return { parseNumber() };

	else if (isMatch(TokenKind::Boolean))
		return { parseBoolean() };

	else if (isMatch(TokenKind::QuotationMark))
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
	require(TokenKind::QuotationMark);
	std::string str = getCurrent();
	require(TokenKind::QuotationMark);
	return str;
}

Array JsonParser::parseArray()
{
	Array arr;
	require(TokenKind::OpenSquare);
	bool new_value = true;
	while (new_value) {
		arr.insert(parseValue());
		new_value = isMatch(TokenKind::Comma);
		if (new_value)
			current++;
	}
	require(TokenKind::CloseSquare);
	return arr;
}

const std::string& JsonParser::getCurrent() 
{
	current++;
	return tokens[current - 1].str;
}

bool JsonParser::isMatch(TokenKind kind)
{
	return kind == tokens[current].kind;
}

void JsonParser::require(TokenKind kind)
{
	if (!isMatch(kind))
		Error::exit("match error: " + tokens[current].str);
	current++;
}
