#include "drawing_mapper.h"

#include <sqlite3.h>
#include <string>
#include <map>
#include "drawing.h"
#include "parser.h"
#include "scanner.h"
#include "builder.h"
#include "painter_mapper.h"

DrawingMapper *DrawingMapper::_instance = nullptr;

DrawingMapper::DrawingMapper()
{
    _builder = new Builder();
    _scanner = new Scanner();
    _parser = new Parser(_scanner, _builder);
}

DrawingMapper::~DrawingMapper()
{
    delete _builder;
    delete _scanner;
    delete _parser;
}

Drawing *DrawingMapper::find(std::string id)
{
    return static_cast<Drawing *>(abstractFind(id, DrawingMapper::callback));
}

void DrawingMapper::add(DomainObject *drawing){
    abstractAdd(drawing);
}

// TODO
void DrawingMapper::update(DomainObject *drawing){
    abstractUpdate(drawing);
}

void DrawingMapper::del(std::string id){
    abstractDelete(id);
}

std::string DrawingMapper::findByIdStmt(std::string id) const
{
    std::string stmt = "SELECT * FROM drawing WHERE ID = '" + id + "'";
    return stmt;
}

std::string DrawingMapper::addStmt(DomainObject *domainObject) const {
    Drawing *drawing = static_cast<Drawing *>(domainObject);
    std::string stmt = "INSERT INTO drawing (ID, painter, shapes) VALUES ('" + drawing->id() + "', '" + drawing->painter()->id() + "', '" + drawing->getShapesAsString() + "')";
    return stmt;
}

std::string DrawingMapper::updateStmt(DomainObject *domainObject) const {
    Drawing *drawing = static_cast<Drawing *>(domainObject);
    std::string stmt = "UPDATE drawing SET painter = '" + drawing->painter()->id() + "', shapes = '" + drawing->getShapesAsString() + "' WHERE ID = '" + drawing->id() + "'";
    return stmt;
}

std::string DrawingMapper::deleteByIdStmt(std::string id) const {
    std::string stmt = "DELETE FROM drawing WHERE ID = '" + id + "'";
    return stmt;
}

DrawingMapper *DrawingMapper::instance(){
    if (_instance == nullptr){
        _instance = new DrawingMapper();
    }
    return _instance;
}   

int DrawingMapper::callback(void *notUsed, int argc, char **argv, char **colNames){
    Painter * painter = PainterMapper::instance()->find(argv[1]);
    Drawing * drawing = new Drawing(argv[0], painter, DrawingMapper::instance()->convertShapes(argc, argv));
    DrawingMapper::instance()->load(drawing);
    return 0;
}

std::list<Shape *> DrawingMapper::convertShapes(int argc, char **argv)
{
    _parser->clear();
    _parser->setInput(argv[2]);
    _parser->parse();
    return _parser->getShapes();
}

