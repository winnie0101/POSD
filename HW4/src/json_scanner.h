#pragma once

#include <string>
#include <sstream>

class JsonScanner {
public:
    void setInput(std::string input) {
        _input = input;
        ss.str(_input);
    }

    char next() {
        char token;
        ss.get(token);

        return token;
    }
    
    bool isDone() const {
        return ss.eof();
    }

private:
    std::string _input;
    std::stringstream ss;
};
