#include <gtest/gtest.h>

#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/link.h"
#include "../src/find_by_name_visitor.h"
#include "../src/tree_visitor.h"
#include "../src/stream_out_visitor.h"

using namespace std;

class LinkTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        home = new Folder("structure/home");

        profile = new File("structure/home/my_profile");
        home->add(profile);

        hello1 = new File("structure/home/hello.txt");
        home->add(hello1);

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

        hello2 = new File("structure/home/hello.txt");
        home->add(hello2);

        download = new Folder("structure/home/Downloads");
        home->add(download);

        funny = new File("structure/home/Downloads/funny.png");
        download->add(funny);

        visitor_folder = new Folder("structure/visitor");
        file1 = new File("structure/visitor/file1.txt");
        visitor_folder->add(file1);
        file2 = new File("structure/visitor/file2.txt");
        visitor_folder->add(file2);
        nested = new Folder("structure/visitor/nested");
        visitor_folder->add(nested);
        file3 = new File("structure/visitor/nested/file3.txt");
        nested->add(file3);
        file4 = new File("structure/visitor/nested/file4.txt");
        nested->add(file4);
        

        lhome = new Link("structure/home", home);
        lprofile = new Link("structure/home/my_profile", profile);
        lhello1 = new Link("structure/home/hello.txt", hello1);
        ldownload = new Link("structure/home/Downloads", download);
        ldocument = new Link("structure/home/Documents", document);
        lfavorite = new Link("structure/home/Documents/favorites", favorite);
        lddd = new Link("structure/home/Documents/favorites/domain-driven-design.pdf", ddd);
        lca = new Link("structure/home/Documents/favorites/clean-architecture.pdf", ca);
        lcqrs = new Link("structure/home/Documents/favorites/cqrs.pdf", cqrs);
        lnote = new Link("structure/home/Documents/note.txt", note);
        lhello2 = new Link("structure/home/hello.txt", hello2);

        lvisitor_folder = new Link("structure/visitor", visitor_folder);
        lfile1 = new Link("structure/visitor/file1.txt", file1);
        lfile2 = new Link("structure/visitor/file2.txt", file2);
        lnested = new Link("structure/visitor/nested", nested);
        lfile3 = new Link("structure/visitor/nested/file3.txt", file3);
        lfile4 = new Link("structure/visitor/nested/file4.txt", file4);

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
        hello1 = nullptr;
        hello2 = nullptr;
        visitor_folder = nullptr;
        file1 = nullptr;
        file2 = nullptr;
        nested = nullptr;
        file3 = nullptr;
        file4 = nullptr;

        lhome = nullptr;
        lprofile = nullptr;
        ldownload = nullptr;
        ldocument = nullptr;
        lnote = nullptr;
        lfavorite = nullptr;
        lddd = nullptr;
        lca = nullptr;
        lcqrs = nullptr;
        lfunny = nullptr;
        lhello1 = nullptr;
        lhello2 = nullptr;

        lvisitor_folder = nullptr;
        lfile1 = nullptr;
        lfile2 = nullptr;
        lnested = nullptr;
        lfile3 = nullptr;
        lfile4 = nullptr;
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
    Node * hello1;
    Node * hello2;

    Node * visitor_folder;
    Node * file1;
    Node * file2;
    Node * nested;
    Node * file3;
    Node * file4;

    Node * lhome;
    Node * lprofile;
    Node * ldownload;
    Node * ldocument;
    Node * lnote;
    Node * lfavorite;
    Node * lddd;
    Node * lca;
    Node * lcqrs;
    Node * lfunny;
    Node * lhello1;
    Node * lhello2;

    Node * lvisitor_folder;
    Node * lfile1;
    Node * lfile2;
    Node * lnested;
    Node * lfile3;
    Node * lfile4;

};

TEST_F(LinkTest, normal) {
    ASSERT_EQ(8, lhome->numberOfFiles());

    Node * result = lhome->find("structure/home/Documents/favorites/cqrs.pdf");
    ASSERT_EQ(cqrs, result);

    std::list<string> result2 = lhome->findByName("cqrs.pdf");
    ASSERT_EQ(1, result2.size());
    ASSERT_EQ("structure/home/Documents/favorites/cqrs.pdf", *(result2.begin()));
}

TEST_F(LinkTest, findByNameVisitor) {
    FindByNameVisitor * visitor = new FindByNameVisitor("clean-architecture.pdf");

    lhome->accept(visitor);
    ASSERT_EQ(1, visitor->getPaths().size());
    ASSERT_EQ(lca->path(), visitor->getPaths().begin().operator*());

    delete visitor;
}

TEST_F(LinkTest, findByNameVisitor2){
    FindByNameVisitor * visitor = new FindByNameVisitor("hello.txt");

    lhome->accept(visitor);
    ASSERT_EQ(2, visitor->getPaths().size());
    delete visitor;
}

TEST_F(LinkTest, streamOutFile){
    StreamOutVisitor * visitor = new StreamOutVisitor();

    lprofile->accept(visitor);

    string expected;
    expected += "_____________________________________________\n";
    expected += "structure/home/my_profile\n";
    expected += "---------------------------------------------\n";
    expected += "Profile\n";
    expected += "Name: name\n";
    expected += "_____________________________________________\n";

    ASSERT_EQ(expected, visitor->getResult());
    delete visitor;
}

TEST_F(LinkTest, streamOutFolder){
    StreamOutVisitor * visitor = new StreamOutVisitor();

    lnested->accept(visitor);

     string expected;
    expected += "_____________________________________________\n";
    expected += "structure/visitor/nested/file3.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 3\n";
    expected += "_____________________________________________\n";
    expected += "\n";
    expected += "_____________________________________________\n";
    expected += "structure/visitor/nested/file4.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 4\n";
    expected += "_____________________________________________\n";
    expected += "\n";

    ASSERT_EQ(expected, visitor->getResult());
    delete visitor;
}

TEST_F(LinkTest, streamOutNestedFolder){
    StreamOutVisitor * visitor = new StreamOutVisitor();

    visitor_folder->accept(visitor);

    string expected;
    expected += "_____________________________________________\n";
    expected += "structure/visitor/file1.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 1\n";
    expected += "_____________________________________________\n";
    expected += "\n";
    expected += "_____________________________________________\n";
    expected += "structure/visitor/file2.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 2\n";
    expected += "_____________________________________________\n";
    expected += "\n";
    expected += "_____________________________________________\n";
    expected += "structure/visitor/nested/file3.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 3\n";
    expected += "_____________________________________________\n";
    expected += "\n";
    expected += "_____________________________________________\n";
    expected += "structure/visitor/nested/file4.txt\n";
    expected += "---------------------------------------------\n";
    expected += "I am file 4\n";
    expected += "_____________________________________________\n";
    expected += "\n";

    ASSERT_EQ(expected, visitor->getResult());
    delete visitor;
}