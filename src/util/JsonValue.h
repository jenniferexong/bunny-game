#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

class Array;
class Object;
using Bool = bool;
using Number = float;
using String = std::string;

using JsonValue = std::variant<Bool, String, Number, Array, Object>;

class Array {
private:
	std::vector<JsonValue> values;

public:
	void insert(const JsonValue& value)
	{
		values.emplace_back(value);
	}
};

class Object {
private:
	std::unordered_map<std::string, JsonValue> values;

public:
	void insert(const std::string& key, const JsonValue& value)
	{
		values.insert({key, value});
	}
};
