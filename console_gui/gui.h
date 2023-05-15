#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <atomic>
#include <chrono>

#include <thread>
#include <mutex>

#include "../db_manager/db_manager.h"
#include "../protocol/data_protocol.h"
#include "flags.h"

const std::string connection_string = "dbname=messenger_db user=admin password=root host=db";
LocalDBManager db;

void index();
std::vector<std::string> signup();
std::vector<std::string> login();
void home(const std::string &username);
