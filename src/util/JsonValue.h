#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

#include "../util/Log.h"

class Array;
class Object;
using Bool = bool;
using Number = float;
using String = std::string;
using JsonValue = std::variant<Bool, String, Number, Array, Object>;

class Array {
private:
	std::vector<JsonValue> values_;

public:
	void insert(const JsonValue& value);
    const std::vector<JsonValue>& getValues() const;
};

class Object {
private:
	std::unordered_map<std::string, JsonValue> values_;

public:
	void insert(const std::string& key, const JsonValue& value);
    bool hasKey(const std::string& key) const;
    const JsonValue& get(const std::string& key) const;
    void require(const std::vector<std::string> keys) const;
};

template<typename T>
bool has(const JsonValue& value) 
{
    if (std::get_if<T>(&value) != nullptr) 
        return true;

    return false;
}

template<typename T>
const T& get(const JsonValue& value)
{
    if (!has<T>(value))
        Error::exit("JsonValue: getting incorrect value");

    return std::get<T>(value);
}

Object getObject(const Object& obj, const std::string& key);
std::vector<float> getNumbers(const Object& obj, const std::string& key);
