#include "JsonValue.h"

Object getObject(const Object& obj, const std::string& key)
{
    return get<Object>(obj.get(key));
}

std::vector<float> getNumbers(const Object& obj, const std::string& key)
{
    std::vector<JsonValue> nums = get<Array>(obj.get(key)).getValues();
    std::vector<float> ans;
    for (auto n: nums)
        ans.push_back(get<Number>(n));
    return ans;
}

void Array::insert(const JsonValue& value)
{
    values_.emplace_back(value);
}

const std::vector<JsonValue>& Array::getValues() const
{
    return values_;
}

void Object::insert(const std::string& key, const JsonValue& value)
{
    values_.insert({key, value});
}

bool Object::hasKey(const std::string& key) const
{
    return values_.find(key) != values_.end();
}

const JsonValue& Object::get(const std::string& key) const
{
    return values_.at(key);
}

void Object::require(const std::vector<std::string> keys) const 
{
    for (auto k: keys)
        if (!hasKey(k))
            Error::exit("Json Object missing key: " + k);
}
