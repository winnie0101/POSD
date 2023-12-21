#pragma once
#include  <stack>

#include "string_value.h"
class JsonBuilder {
private:
    std::stack<std::pair<std::string, JsonObject*>> _objStack;
    JsonObject * rootObject;
public:
    // JsonBuilder(): _currentJsonObj(nullptr){};

    void buildValue(std::string key, std::string value) {
        StringValue * stringValue = new StringValue(value);
        if(!_objStack.empty()){
            _objStack.top().second->set(key, stringValue);
        } else {
            if(rootObject == nullptr){
                rootObject = new JsonObject();
            }
             rootObject->set(key, stringValue);
        }
        // std::cout<<"rootObject: "<<rootObject->toString()<<"\n";
    };

    void buildObject(std::string key) {
        JsonObject * newJsonObj = new JsonObject();
        if(!_objStack.empty()){
            _objStack.top().second->set(key, newJsonObj);
        } else {
            rootObject = newJsonObj;
        }
        _objStack.push({key, newJsonObj});
    };

    void endObject() {
        if (!_objStack.empty()) {
            _objStack.pop();
        }
    };

    JsonObject * getJsonObject() {
        return rootObject;
    };
};
