#pragma once
#include <string>
#include <sqlite3.h>

#ifndef SERVER_DB_H
#define SERVER_DB_H

bool initializeDatabase();
void logAccess(const std::string& user, const std::string& action);

#endif
