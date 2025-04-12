#include "server_logger.h"
#include <fstream>
#include <ctime>

#define LOG_FILE_PATH "server/server_log.txt"

namespace ServerLogger {

    void logEvent(const std::string& event) {
        std::ofstream logFile(LOG_FILE_PATH, std::ios::app);
        if (logFile.is_open()) {
            std::time_t now = std::time(nullptr);
            char* dt = std::ctime(&now);
            dt[strlen(dt) - 1] = '\0';  // Remove newline
            logFile << dt << ": " << event << std::endl;
            logFile.close();
        }
    }

}
