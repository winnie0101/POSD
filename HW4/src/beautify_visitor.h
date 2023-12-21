#pragma once
#include <string>
#include "visitor.h"

class BeautifyVisitor : public JsonVisitor {
private:
    std::string _result;
    int _currentIndent;
    void appendIndent() {
        for(int i=0; i<_currentIndent; ++i){
            _result += " ";
        }
    }
public:
    BeautifyVisitor():_result(""), _currentIndent(0) {};

    void visitJsonObject(JsonObject * obj) override {
        _result += "{\n";
        _currentIndent += 4;

        JsonIterator * it = obj->createIterator();
        int count = 0;
        for(it->first(); !it->isDone(); it->next()){
            int _objSize = it->count();
            appendIndent();
            _result += "\"" + it->currentKey() + "\": ";

            Value * value = it->currentValue();
            count++;
            value->accept(this);

            if(count != _objSize) {
                _result += ",";
            }
            _result += "\n";
            delete value;
        }
        _currentIndent -= 4;
        appendIndent();
        _result += "}";
        delete it;
    };

    void visitStringValue(StringValue * val) override {
        _result += val->toString();
    };

    std::string getResult() const {
        return _result;
    };
};
       