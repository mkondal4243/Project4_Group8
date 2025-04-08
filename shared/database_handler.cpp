#include "database_utils.h"
#include <sqlite3.h>
#include <iostream>

static sqlite3* db = nullptr;

bool DatabaseUtils::initializeDB(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "SQLite Error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* createLoginTable =
        "CREATE TABLE IF NOT EXISTS LoginAttempts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "result TEXT NOT NULL,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    const char* createMotionTable =
        "CREATE TABLE IF NOT EXISTS MotionAlerts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "message TEXT NOT NULL,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    const char* createGeneralTable =
        "CREATE TABLE IF NOT EXISTS GeneralLogs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "message TEXT NOT NULL,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    char* errMsg = nullptr;
    sqlite3_exec(db, createLoginTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, createMotionTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, createGeneralTable, nullptr, nullptr, &errMsg);

    if (errMsg) {
        std::cerr << " SQLite Table Creation Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

void DatabaseUtils::logLoginAttempt(const std::string& username, const std::string& result) {
    std::string sql = "INSERT INTO LoginAttempts (username, result) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, result.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void DatabaseUtils::logMotionAlert(const std::string& alertMessage) {
    std::string sql = "INSERT INTO MotionAlerts (message) VALUES (?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, alertMessage.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void DatabaseUtils::logGeneralEvent(const std::string& eventMessage) {
    std::string sql = "INSERT INTO GeneralLogs (message) VALUES (?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, eventMessage.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
