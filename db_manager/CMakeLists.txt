cmake_minimum_required(VERSION 3.26)
project(db_manager_example)

# Set the C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Postgres stuff
set(PostgreSQL_ADDITIONAL_VERSIONS "13")
set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/local/Cellar/postgresql/13.3/include/server")
set(PostgreSQL_TYPE_LIBRARY_DIR "/usr/local/Cellar/postgresql/13.3/lib")

# # Include the discovered directories
# include_directories(${PQXX_INCLUDE_DIRECTORIES})
# include_directories(/usr/local/include)

# # Add the pqxx library
# find_path(libpqxx_INCLUDE_DIR /pqxx/pqxx)
# include(SelectLibraryConfigurations)
# find_library(libpqxx_LIBRARY_RELEASE NAMES libpqxx)
# find_library(libpqxx_LIBRARY_DEBUG NAMES libpqxxd)
# select_library_configurations(libpqxx)

# Add the executable for the main program
add_executable(db_manager main.cpp db_manager.cpp)

# # Link the pqxx library
# target_link_libraries(db_manager pqxx)

# Set the output directories for the executables
set_target_properties(db_manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
