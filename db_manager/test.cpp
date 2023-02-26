#include "gtest/gtest.h"
#include "db_manager.h"

// Test case to ensure that the connection to the database is successful
TEST(TDBManagerTest, ConnectsToDatabase) {
    TDBManager db_manager("testdb", "testtable");
    ASSERT_TRUE(db_manager.PathIsValid("testdb"));
}

// Test case to ensure that data can be posted to the database
TEST(TDBManagerTest, PostsDataToDatabase) {
    TDBManager db_manager("testdb", "testtable");
    db_manager.PostData("Test Data");
    std::string result = db_manager.GetData();
    ASSERT_EQ(result, "Test Data\n");
}

// Test case to ensure that data can be retrieved from the database
TEST(TDBManagerTest, RetrievesDataFromDatabase) {
    TDBManager db_manager("testdb", "testtable");
    db_manager.PostData("Test Data");
    std::string result = db_manager.GetData();
    ASSERT_EQ(result, "Test Data\n");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}