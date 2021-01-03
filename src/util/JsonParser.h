#pragma once

#include <regex>

#include "JsonValue.h"

class JsonParser {
private:
	static std::vector<std::string> tokens;

	static int current;

	static std::regex curly_brace_open;
	static std::regex curly_brace_close;
	static std::regex square_brace_open;
	static std::regex square_brace_close;
	static std::regex quotation_mark;
	static std::regex number;
	static std::regex comma;
	static std::regex colon;
	static std::regex boolean;

	static Bool parseBoolean();
	static Number parseNumber();
	static String parseString();
	static String parseKey();
	static Object parseObject();
	static Array parseArray();
	static JsonValue parseValue();

	static const std::string& getCurrent();
	static bool isMatch(std::regex pattern);
	static void require(std::regex pattern);

public:
	static JsonValue parse(const std::string& file_name);
};

