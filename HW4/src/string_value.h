#pragma once
#include "value.h"
#include "visitor.h"

class StringValue : public Value {
private:
    std::string _value;

public:
    StringValue(std::string value) : _value(value) {};

    std::string toString() const override {
        return "\"" + _value + "\"";
    };

    void accept(JsonVisitor * visitor) override {
        visitor->visitStringValue(this);
    }
};