cmake_minimum_required(VERSION 3.24)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

# Set up pqxx library
find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})
add_definitions(${PostgreSQL_DEFINITIONS})

# Add db_manager directory and files
add_library(DBManager db_manager/db_manager.cpp db_manager/db_manager.h)
target_link_libraries(DBManager ${PostgreSQL_LIBRARIES})

# Add test executable
add_executable(test_db_manager db_manager/test.cpp)
target_link_libraries(test_db_manager DBManager)

# Set default target to test executable
add_custom_target(default_target ALL COMMAND test_db_manager)
