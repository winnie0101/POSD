#include <gtest/gtest.h>

#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/iterator.h"
#include "../src/iterator_factories.h"

using namespace std;

class IteratorFactoryTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        home = new Folder("structure/home");

        profile = new File("structure/home/my_profile");
        home->add(profile);

        document = new Folder("structure/home/Documents");
        home->add(document);

        favorite = new Folder("structure/home/Documents/favorites");
        document->add(favorite);
        ddd = new File("structure/home/Documents/favorites/domain-driven-design.pdf");
        favorite->add(ddd);
        ca = new File("structure/home/Documents/favorites/clean-architecture.pdf");
        favorite->add(ca);
        cqrs = new File("structure/home/Documents/favorites/cqrs.pdf");
        favorite->add(cqrs);

        note = new File("structure/home/Documents/note.txt");
        document->add(note);

        download = new Folder("structure/home/Downloads");
        home->add(download);

        funny = new File("structure/home/Downloads/funny.png");
        download->add(funny);

        hello = new File("structure/home/hello.txt");
        home->add(hello);
    }

    void TearDown() {
        delete home;
        home = nullptr;
        profile = nullptr;
        download = nullptr;
        document = nullptr;
        note = nullptr;
        favorite = nullptr;
        ddd = nullptr;
        ca = nullptr;
        cqrs = nullptr;
        funny = nullptr;
        hello = nullptr;
    }
    
    Node * home;
    Node * profile;
    Node * download;
    Node * document;
    Node * note;
    Node * favorite;
    Node * ddd;
    Node * ca;
    Node * cqrs;
    Node * funny;
    Node * hello;
};

TEST_F(IteratorFactoryTest, defaultIteratorFactory) {
    Iterator * it = home->createIterator();
    it->first();
    ASSERT_FALSE(it->isDone());
    ASSERT_EQ("my_profile", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Documents", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Downloads", it->currentItem()->name());

    it->next();
    ASSERT_EQ("hello.txt", it->currentItem()->name());

    it->next();
    ASSERT_TRUE(it->isDone());

    delete it;
}

TEST_F(IteratorFactoryTest, folderIteratorFactory) {
    IteratorFactory * iteratorFactory = new FolderIteratorFactory();
    Iterator * it = home->createIterator(iteratorFactory);

    it->first();
    ASSERT_FALSE(it->isDone());
    ASSERT_EQ("my_profile", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Documents", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Downloads", it->currentItem()->name());

    it->next();
    ASSERT_EQ("hello.txt", it->currentItem()->name());

    it->next();
    ASSERT_TRUE(it->isDone());

    delete it;
    delete iteratorFactory;
}

TEST_F(IteratorFactoryTest, orderByNameIteratorFactory) {
    IteratorFactory * iteratorFactory = new OrderByNameIteratorFactory();
    Iterator * it = home->createIterator(iteratorFactory);

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

    delete it;
    delete iteratorFactory;
}

TEST_F(IteratorFactoryTest, orderByNameWithFolderFirstIteratorFactory) {
    IteratorFactory * iteratorFactory = new OrderByNameWithFolderFirstIteratorFactory();
    Iterator * it = home->createIterator(iteratorFactory);

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

    delete it;
    delete iteratorFactory;
}

TEST_F(IteratorFactoryTest, OrderByKindIteratorFactory) {
    IteratorFactory * iteratorFactory = new OrderByKindIteratorFactory();
    Iterator * it = home->createIterator(iteratorFactory);

    it->first();
    ASSERT_FALSE(it->isDone());
    ASSERT_EQ("my_profile", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Documents", it->currentItem()->name());

    it->next();
    ASSERT_EQ("Downloads", it->currentItem()->name());

    it->next();
    ASSERT_EQ("hello.txt", it->currentItem()->name());

    it->next();
    ASSERT_TRUE(it->isDone());

    delete it;
    delete iteratorFactory;
}