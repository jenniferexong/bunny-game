#pragma once

#include <string>
#include <vector>
#include <cctype>

enum class TokenKind {
    Boolean, Number, String,
    // punctuation
    OpenBrace, CloseBrace, OpenSquare, CloseSquare, QuotationMark, Comma, Colon
};

struct Token {
    TokenKind kind;
    std::string str;

    Token() = default;
    Token(TokenKind k, const std::string& s): kind(k), str(s) {}
};

struct JsonTokens {

	std::vector<Token> tokens;

	static constexpr char open_brace = '{';
	static constexpr char close_brace = '}';
	static constexpr char open_square = '[';
	static constexpr char close_square = ']';
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
    int getPunctuation(int start_index, const char& c);

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
			|| c == open_brace
			|| c == close_brace
			|| c == open_square
			|| c == close_square
			|| c == comma
			|| c == colon
		);
	}
};
