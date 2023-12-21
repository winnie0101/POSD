#pragma once
#include "value.h"
#include "json_iterator.h"

#include<map>

class JsonObject : public Value {
private:
    std::map<std::string, Value*> _jsonObj;

public:
    ~JsonObject() {
        for(auto entry : _jsonObj) {
            delete & entry.second;
        }
    }

    void set(std::string key, Value *value) {
        _jsonObj[key] = value;
    };

    std::string toString() const override{
        std::string result = "{\n";
        for(auto it=_jsonObj.begin(); it!=_jsonObj.end(); ++it) {
            result += "\"" + it->first + "\":" + it->second->toString();
            if(std::next(it) != _jsonObj.end()){
                result += ",\n";
            }
        }
        result += "\n}";
        return result;
    };

    Value * getValue(std::string key) {
        auto it = _jsonObj.find(key);
        if( it != _jsonObj.end()){
            return it->second;
        } else {
            throw string ("this key does not exist");
        }
    };
    
    // Factory method for creating JsonIterator
    JsonIterator * createIterator() override {
        return new JsonObjectIterator(_jsonObj);
    };

    //Abstract factory method for creating JsonIterator
    JsonIterator * createIterator(IteratorFactory * factory) override {
        return factory->create(_jsonObj);
    };

    void accept(JsonVisitor * visitor) override {
        visitor->visitJsonObject(this);
    };
};