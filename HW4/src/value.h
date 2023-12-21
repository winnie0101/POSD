#pragma once
#include <string>
#include "json_iterator.h"
#include "iterator_factory.h"
#include "visitor.h"

class Value {
public:
    virtual std::string toString() const = 0;

    virtual JsonIterator * createIterator() {
        return new NullIterator();
    };

    virtual JsonIterator * createIterator(IteratorFactory * factory) {
        return new NullIterator();
    };

    virtual void accept(JsonVisitor * visitor) {};
};