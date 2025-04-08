#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <string>

class DatabaseUtils {
public:
    static bool initializeDB(const std::string& dbName);
    static void logLoginAttempt(const std::string& username, const std::string& result);
    static void logMotionAlert(const std::string& alertMessage);
    static void logGeneralEvent(const std::string& eventMessage);
};

#endif
