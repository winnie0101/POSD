#pragma once

#include <unordered_map>
#include "domain_object.h"
#include "unit_of_work.h"
#include <iostream>
#include <sqlite3.h>

typedef int (*CallbackType)(void *, int argc, char **argv, char **col_names);

class AbstractMapper
{
public:
    virtual ~AbstractMapper()
    {
    }

    // Template methods for building SQL statements
    virtual std::string updateStmt(DomainObject *domainObject) const = 0;
    virtual std::string findByIdStmt(std::string id) const = 0;
    virtual std::string addStmt(DomainObject *domainObject) const = 0;
    virtual std::string deleteByIdStmt(std::string id) const = 0;

    void setDB(std::string db_file)
    {
        int result = sqlite3_open(db_file.c_str(), &_db);
        if (result != SQLITE_OK){
            std::cerr << "Can't open database: " << sqlite3_errmsg(_db) << "\n";
            exit(0);
        }
    }

    bool isLoaded(const std::string &id) const
    {
        return _domainObjects.find(id) != _domainObjects.end();
    }

    void cleanCache()
    {
        _domainObjects.clear();
    }

protected:
    std::unordered_map<std::string, DomainObject *> _domainObjects;
    sqlite3 *_db;

    DomainObject *getDomainObject(std::string id)
    {
        if (_domainObjects.count(id))
        {
            return _domainObjects.at(id);
        }
        return nullptr;
    }

    DomainObject * abstractFind(std::string id, CallbackType callback) {
        DomainObject *domainObject = getDomainObject(id);
        if(domainObject != nullptr){
            return domainObject;
        }

        sqlite3_exec(_db, findByIdStmt(id).c_str(), callback, NULL, &_errMsg);

        DomainObject * resultObj = getDomainObject(id);
        if(resultObj!=nullptr) {
            UnitOfWork::instance()->registerClean(resultObj);
        }
        return resultObj;
    };

    void abstractAdd(DomainObject *domainObject) {
        sqlite3_exec(_db, addStmt(domainObject).c_str(), NULL, NULL, &_errMsg);
    };

    void abstractUpdate(DomainObject *domainObject) {
        sqlite3_exec(_db, updateStmt(domainObject).c_str(), NULL, NULL, &_errMsg);
    };

    void abstractDelete(std::string id) {
        sqlite3_exec(_db, deleteByIdStmt(id).c_str(), NULL, NULL, &_errMsg);
        cleanCache();
    };

    void load(DomainObject *domainObject) {
        _domainObjects[domainObject->id()] = domainObject;
    };

private:
    char *_errMsg;
};