#pragma once

#include "folder.h"
#include "iterator_factory.h"

using namespace std;


class FolderIteratorFactory: public IteratorFactory {
    public:

        static FolderIteratorFactory * instance() {
            if(_instance == nullptr){
                _instance = new FolderIteratorFactory();
            }
            return _instance;
        }

        Iterator * create(Folder * node, int operationCount) override {
            return new Folder::FolderIterator(node, operationCount);
        }

    private:
        static FolderIteratorFactory * _instance;
};

class OrderByNameIteratorFactory: public IteratorFactory {
    public:
        
        static OrderByNameIteratorFactory * instance() {
            if(_instance == nullptr){
                _instance = new OrderByNameIteratorFactory();
            }
            return _instance;
        }

        Iterator * create(Folder * node, int operationCount) override {
            return new Folder::OrderByNameIterator(node, operationCount);
        }

    private:
        static OrderByNameIteratorFactory * _instance;
};

class OrderByNameWithFolderFirstIteratorFactory: public IteratorFactory {
    public:

        static OrderByNameWithFolderFirstIteratorFactory * instance() {
            if(_instance == nullptr){
                _instance = new OrderByNameWithFolderFirstIteratorFactory();
            }   
            return _instance;
        }

        Iterator * create(Folder * node, int operationCount) override {
            return new Folder::OrderByNameWithFolderFirstIterator(node, operationCount);
        }

    private:
        static OrderByNameWithFolderFirstIteratorFactory * _instance;
};

class OrderByKindIteratorFactory: public IteratorFactory {
    public:

        static OrderByKindIteratorFactory * instance() {
            if(_instance == nullptr){
                _instance = new OrderByKindIteratorFactory();
            }
            return _instance;
        }

        Iterator * create(Folder * node, int operationCount) override {
            return new Folder::OrderByKindIterator(node, operationCount);
        }

    private:
        static OrderByKindIteratorFactory * _instance;
};

FolderIteratorFactory* FolderIteratorFactory::_instance = nullptr;

OrderByNameIteratorFactory* OrderByNameIteratorFactory::_instance = nullptr;

OrderByNameWithFolderFirstIteratorFactory* OrderByNameWithFolderFirstIteratorFactory::_instance = nullptr;

OrderByKindIteratorFactory* OrderByKindIteratorFactory::_instance = nullptr;