#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/iterator.h"
#include "../src/file_system_builder.h"
#include "../src/file_system_parser.h"
#include "../src/file_system_scanner.h"

TEST(FileSystemBuilder, Normal) {
    FileSystemParser * parser = new FileSystemParser(new FileSystemBuilder());
    parser->setPath("structure/home");
    parser->parse();

    Node * home = parser->getRoot();

    IteratorFactory * factory = new OrderByNameIteratorFactory();
    Iterator * it = home->createIterator(factory);
    it->first();
    ASSERT_FALSE(it->isDone());
    
    ASSERT_EQ("Documents", it->currentItem()->name());
    
    it->next();
    ASSERT_EQ("Downloads", it->currentItem()->name());

    it->next();
    ASSERT_EQ("hello.txt", it->currentItem()->name());

    it->next();
    ASSERT_EQ("my_profile", it->currentItem()->name());

    it->next();
    ASSERT_TRUE(it->isDone());

    delete parser;
    delete home;
    delete it;
}