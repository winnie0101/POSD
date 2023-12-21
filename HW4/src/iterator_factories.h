#pragma once

#include "json_object.h"
#include "json_iterator.h"
#include "iterator_factory.h"

#include<map>

using namespace std;

class JsonObjectIteratorFactory: public IteratorFactory {
    public:
        static JsonObjectIteratorFactory * instance(){
            if(_instance == nullptr) {
                _instance = new JsonObjectIteratorFactory();
            }
            return _instance;
        }

        JsonIterator * create(map<std::string, Value*> jsonObj) override {
            return new JsonObjectIterator(jsonObj);
        }
    private:
        static JsonObjectIteratorFactory * _instance;

};

class OrderByNameIteratorFactory: public IteratorFactory {
    public:

        static OrderByNameIteratorFactory * instance() {
            if(_instance == nullptr) {
                _instance = new OrderByNameIteratorFactory();
            }
            return _instance;
        }

        JsonIterator * create(map<std::string, Value*> jsonObj) override {
            return new OrderByNameIterator(jsonObj);
        }

    private:
        static OrderByNameIteratorFactory * _instance;
};

JsonObjectIteratorFactory * JsonObjectIteratorFactory::_instance = nullptr;
OrderByNameIteratorFactory * OrderByNameIteratorFactory::_instance = nullptr;
