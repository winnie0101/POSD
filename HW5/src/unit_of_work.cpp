#include "unit_of_work.h"
#include "drawing_mapper.h"
#include "painter_mapper.h"
#include "domain_object.h"
#include "drawing.h"
#include "painter.h"
#include <list>
#include <iostream>


UnitOfWork * UnitOfWork::_instance = nullptr;

UnitOfWork::UnitOfWork() {}

UnitOfWork * UnitOfWork::instance() {
    if(_instance == nullptr) {
        _instance = new UnitOfWork();
    }
    return _instance;
}

void UnitOfWork::registerNew(DomainObject * domainObject) {
    _new[domainObject->id()] = domainObject;
}

void UnitOfWork::registerClean(DomainObject * domainObject){
    _clean[domainObject->id()] = domainObject;
}

void UnitOfWork::registerDirty(DomainObject * domainObject){
    _dirty[domainObject->id()] = domainObject;
    _clean.erase(domainObject->id());
}

void UnitOfWork::registerDeleted(DomainObject * domainObject) {
    _deleted[domainObject->id()] = domainObject;
    _clean.erase(domainObject->id());
    _new.erase(domainObject->id());
}

bool UnitOfWork::inNew(std::string id) const {
    return _new.find(id) != _new.end();
}

bool UnitOfWork::inClean(std::string id) const {
    return _clean.find(id) != _clean.end();
}

bool UnitOfWork::inDirty(std::string id) const {
    return _dirty.find(id) != _dirty.end();
}

bool UnitOfWork::inDeleted(std::string id) const {
    return _deleted.find(id) != _deleted.end();
}

void UnitOfWork::commit() {
    DrawingMapper * drawingMapper = DrawingMapper::instance();
    PainterMapper * painterMapper = PainterMapper::instance();

    for(auto newObj : _new) {
        DomainObject * obj = newObj.second;
        if(dynamic_cast<Drawing *>(obj)){
            drawingMapper->add(obj);
            _clean[obj->id()] = _new[obj->id()];
        } else if(dynamic_cast<Painter *>(obj)){
            painterMapper->add(obj);
            _clean[obj->id()] = _new[obj->id()];
        }
    }
    _new.clear();

    for(auto dirty : _dirty) {
        DomainObject * obj = dirty.second;
        if(dynamic_cast<Drawing *>(obj)){
            drawingMapper->update(obj);
            _clean[obj->id()] = _new[obj->id()];
        } else if (dynamic_cast<Painter *>(obj)){
            painterMapper->update(obj);
            _clean[obj->id()] = _new[obj->id()];
        }
    }
    _dirty.clear();

    for(auto deleted : _deleted) {
        DomainObject * obj = deleted.second;
        if(dynamic_cast<Drawing *>(obj)) {
            drawingMapper->del(obj->id());
        } else if (dynamic_cast<Painter *>(obj)) {
            painterMapper->del(obj->id());
        }
    }
    _deleted.clear();
}