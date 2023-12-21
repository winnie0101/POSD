#pragma once

#include <string>
#include <sqlite3.h>

#include "abstract_mapper.h"

class Painter;

class PainterMapper: public AbstractMapper {
public:
    Painter* find(std::string id);
    
    void add(DomainObject * Painter);

    void update(DomainObject * Painter);

    void del(std::string id);

    std::string updateStmt(DomainObject * domainObject) const;

    std::string findByIdStmt(std::string id) const;

    std::string addStmt(DomainObject * domainObject) const;

    std::string deleteByIdStmt(std::string id) const;

    static PainterMapper* instance();

protected:
    PainterMapper();

    static int callback(void* notUsed, int argc, char** argv, char** colNames);
    
private:
    char* _errorMessage;
    static PainterMapper* _instance;
};