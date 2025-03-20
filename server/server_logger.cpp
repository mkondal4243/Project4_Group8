#include "server_logger.h"
#include <fstream>
#include <ctime>

namespace ServerLogger {

    void logEvent(const std::string& event) {
        std::ofstream logFile("server/server_log.txt", std::ios::app);
        std::time_t now = std::time(nullptr);
        logFile << std::ctime(&now) << ": " << event << std::endl;
        logFile.close();
    }

}
