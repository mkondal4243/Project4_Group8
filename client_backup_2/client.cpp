#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "client_utils.h"
#include <sys/stat.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define MAX_RETRIES 5
#define LOG_FILE_NAME "logs/received_logs.txt"

bool tryReconnect(int& sock) {
    for (int i = 0; i < MAX_RETRIES; ++i) {
        std::cout << "Attempting to reconnect (" << i + 1 << "/" << MAX_RETRIES << ")...\n";
        sock = ClientUtils::createSocket();
        if (sock >= 0 && ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
            std::cout << "✅ Reconnected successfully.\n";
            return true;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cerr << "❌ Failed to reconnect after " << MAX_RETRIES << " attempts.\n";
    return false;
}

void handleMotionEvent(const std::string& message) {
    if (message.find("MOTION_DETECTED") != std::string::npos) {
        std::cout << "🚨 [Security Alert] Motion Detected! Taking appropriate action...\n";
    }
}

void receiveAndSaveLogFile(int sock) {
    // Ensure logs/ directory exists
    const char* logsDir = "logs";
    struct stat st = {0};
if (stat(logsDir, &st) == -1) {
    mkdir(logsDir, 0777);
}

    std::ofstream logFile(LOG_FILE_NAME);
    if (!logFile.is_open()) {
        std::cerr << "❌ Failed to open log file for writing.\n";
        return;
    }

    std::string logData = ClientUtils::receiveMessage(sock);
    logFile << logData;
    logFile.close();
    std::cout << "✅ Log file saved at '" << LOG_FILE_NAME << "'.\n";
}

int main() {
    int sock = ClientUtils::createSocket();
    if (sock < 0) return -1;

    if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
        if (!tryReconnect(sock)) return -1;
    }

    std::cout << "Connected to SecureLink Server!\n";

    std::string username, password;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::string credentials = username + ":" + password;
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Server Response: " << response << std::endl;

    if (response == "AUTH_SUCCESS") {
        receiveAndSaveLogFile(sock);
        std::string eventMessage = ClientUtils::receiveMessage(sock);
        std::cout << "Event: " << eventMessage << std::endl;
        handleMotionEvent(eventMessage);
    }

    ClientUtils::closeSocket(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
