#pragma once

#include <string>
#include <vector>
#include <cctype>

struct JsonTokens {
	std::vector<std::string> tokens;

	static constexpr char curly_brace_open = '{';
	static constexpr char curly_brace_close = '}';
	static constexpr char square_brace_open = '[';
	static constexpr char square_brace_close = ']';
	static constexpr char quotation_mark = '"';
	static constexpr char comma = ',';
	static constexpr char colon = ':';

	/**
	 * Turns a json file into vector of tokens
	 */
	JsonTokens(const std::string& file_name);

	int getBool(int start_index, std::string& string);
	int getNumber(int start_index, std::string& string);
	int getString(int start_index, std::string& string);

	static bool isQuotation(const char& c) { return c == quotation_mark; }

	static bool isNumber(const char& c) 
	{ 
		return isdigit(c) || c == '.' || c == '-';
	}

	static bool isWhiteSpace(const char& c) 
	{
		return c == ' ' || c == '\t' || c == '\n';
	}

	static bool isPunctuation(const char& c) 
	{ 
		return (
			c == quotation_mark
			|| c == curly_brace_open
			|| c == curly_brace_close
			|| c == square_brace_open
			|| c == square_brace_close
			|| c == comma
			|| c == colon
		);
	}
};
