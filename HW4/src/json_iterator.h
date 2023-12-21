#pragma once
#include <string>
#include <map>
#include <list>
#include <iostream>

using namespace std;

class Value;

class JsonIterator {
public:
    virtual void first() = 0;

    virtual std::string currentKey() = 0;

    virtual Value * currentValue() = 0;

    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual int count() const {
        return 0;
    }
};

class JsonObjectIterator : public JsonIterator {
private:
    std::map<std::string, Value*> _jsonObj;
    std::map<std::string, Value*>::const_iterator _currentObj;
    list<std::string> _keyList;

public:
    JsonObjectIterator(std::map<std::string, Value*> jsonObj) : _jsonObj(jsonObj) {};

    void first() {
        _currentObj = _jsonObj.begin();
    }

    std::string currentKey() {
        return _currentObj->first;
    }

    Value * currentValue() {
        return _currentObj->second;
    }

    void next() {
        if( !isDone() ){
            ++_currentObj;
        }
    }

    bool isDone() {
        return _currentObj == _jsonObj.end();
    }

    virtual int count() const {
        return _jsonObj.size();
    }
};

class OrderByNameIterator : public JsonIterator {
private:
    std::map<std::string, Value*> _jsonObj;
    std::map<std::string, Value*>::const_iterator _currentObj;
    std::map<std::string, Value*, greater<string>> _jsonObjSort;
    list<std::string> _keyList;

public:
    OrderByNameIterator(std::map<std::string, Value*> jsonObj) : _jsonObj(jsonObj) {};

    void first() {
        for(auto it=_jsonObj.begin(); it!=_jsonObj.end(); ++it){
            _jsonObjSort[it->first] = it->second;
        }

        _currentObj = _jsonObjSort.begin();
    }

    std::string currentKey() {
        return _currentObj->first;
    }

    Value * currentValue() {
        return _currentObj->second;
    }

    void next() {
        if( !isDone() ){
            ++_currentObj;
        }
    }

    bool isDone() {
        return _currentObj == _jsonObjSort.end();
    }

    virtual int count() const {
        return _jsonObj.size();
    }
};

class NullIterator : public JsonIterator {

public:
    void first() override {};

    std::string currentKey() override{
        throw std::string("");
    }

    Value * currentValue() override {
        throw std::string("");
    }

    void next() override {}

    bool isDone() override {
        return true;
    }
};