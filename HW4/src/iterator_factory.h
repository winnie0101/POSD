#pragma once

#include "json_iterator.h"

#include<map>

using namespace std;

class JsonObject;

class IteratorFactory {
    public:
        virtual JsonIterator * create(map<std::string, Value*> jsonObject) = 0;
};