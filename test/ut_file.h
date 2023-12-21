#include "../src/file.h"
using namespace std;

TEST(File, normal) {
    File hello("structure/file.txt");
    ASSERT_EQ("file.txt", hello.name());
    ASSERT_EQ("structure/file.txt", hello.path());
}


TEST(File, invalid_file) {
    ASSERT_ANY_THROW(File("/NOT/EXIST/PATH"));
}

TEST(File, invalid_file_2) {
    ASSERT_ANY_THROW(File("structure/home"));
}

TEST(File, rename){
    File * hello = new File("structure/home/hello.txt");
    hello->rename("new_hello.txt");
    ASSERT_EQ("structure/home/new_hello.txt", hello->path());
}