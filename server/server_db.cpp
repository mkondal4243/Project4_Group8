#include <sqlite3.h>
#include <iostream>

bool initializeDatabase() {
    sqlite3* db;
    int rc = sqlite3_open("securelink.db", &db);

    if (rc) {
        std::cerr << "❌ Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS access_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            timestamp TEXT,
            action TEXT
        );
    )";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "❌ SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    sqlite3_close(db);
    return true;
}

void logAccess(const std::string& user, const std::string& action) {
    sqlite3* db;
    sqlite3_open("securelink.db", &db);

    std::string query = "INSERT INTO access_logs (username, timestamp, action) VALUES ('" +
                        user + "', datetime('now'), '" + action + "');";

    char* errMsg = nullptr;
    sqlite3_exec(db, query.c_str(), 0, 0, &errMsg);
    sqlite3_close(db);
}
