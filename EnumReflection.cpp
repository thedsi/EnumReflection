// (c) 2016 Yakov Litvitskiy <thedsi100@gmail.com>
// See LICENSE.txt
#include "EnumReflection.h"
#include <vector>
#include <cassert>

struct EnumReflector::Private
{
    struct Enumerator
    {
        std::string name;
        int value;
    };
    std::vector<Enumerator> values;
    std::string enumName;
};

static bool IsIdentChar(char c)
{
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

EnumReflector::EnumReflector(const int* vals, int count, const char* name, const char* body)
    : _data(new Private)
{
    _data->enumName = name;
    _data->values.resize(count);
    enum states
    {
        state_start, // Before identifier
        state_ident, // In identifier
        state_skip, // Looking for separator comma
    } state = state_start;
    assert(*body == '(');
    ++body;
    const char* ident_start = nullptr;
    int value_index = 0;
    int level = 0;
    for (;;)
    {
        assert(*body);
        switch (state)
        {
        case state_start:
            if (IsIdentChar(*body))
            {
                state = state_ident;
                ident_start = body;
            }
            ++body;
            break;
        case state_ident:
            if (!IsIdentChar(*body))
            {
                state = state_skip;
                assert(value_index < count);
                _data->values[value_index].name = std::string(ident_start, body - ident_start);
                _data->values[value_index].value = vals[value_index];
                ++value_index;
            }
            else
            {
                ++body;
            }
            break;
        case state_skip:
            if (*body == '(')
            {
                ++level;
            }
            else if (*body == ')')
            {
                if (level == 0)
                {
                    assert(value_index == count);
                    return;
                }
                --level;
            }
            else if (level == 0 && *body == ',')
            {
                state = state_start;
            }
            ++body;
        }
    }
}

EnumReflector::~EnumReflector()
{
    if(_data)
        delete _data;
}

int EnumReflector::Count() const
{
    return (int)_data->values.size();
}

EnumReflector::Enumerator EnumReflector::Find(const std::string& name) const
{
    for (int i = 0; i < (int)_data->values.size(); ++i)
    {
        if (_data->values[i].name == name)
            return At(i);
    }
    return end();
}

EnumReflector::Enumerator EnumReflector::Find(int value) const
{
    for (int i = 0; i < (int)_data->values.size(); ++i)
    {
        if (_data->values[i].value == value)
            return At(i);
    }
    return end();
}

const std::string& EnumReflector::EnumName() const
{
    return _data->enumName;
}

const std::string& EnumReflector::Enumerator::Name() const
{
    return _er._data->values[_index].name;
}

int EnumReflector::Enumerator::Value() const
{
    return _er._data->values[_index].value;
}
