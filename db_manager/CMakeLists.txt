cmake_minimum_required(VERSION 3.1)

project(DBManager)

# Add the pqxx library
find_package(pqxx REQUIRED)

# Add the executable for the main program
add_executable(db_manager main.cpp db_manager.cpp)

# Link the pqxx library
target_link_libraries(db_manager pqxx)

# Add the executable for the tests
add_executable(db_manager_test db_manager.cpp db_manager_test.cpp)

# Link the pqxx library and the Google Test framework
target_link_libraries(db_manager_test pqxx gtest_main)

# Add the C++11 flag
set(CMAKE_CXX_STANDARD 11)

# Set the output directories for the executables
set_target_properties(db_manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(db_manager_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Add the tests to the CTest framework
add_test(NAME TDBManagerTest COMMAND db_manager_test)
