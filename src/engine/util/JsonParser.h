#pragma once

#include "JsonValue.h"
#include "JsonTokens.h"

struct Token;

class JsonParser {
private:
	static std::vector<Token> tokens;

	static int current;

	static Bool parseBoolean();
	static Number parseNumber();
	static String parseString();
	static String parseKey();
	static Object parseObject();
	static Array parseArray();
	static JsonValue parseValue();

	static const std::string& getCurrent();
	static bool isMatch(TokenKind kind);
	static void require(TokenKind kind);

public:
	static JsonValue parse(const std::string& file_name);
};

