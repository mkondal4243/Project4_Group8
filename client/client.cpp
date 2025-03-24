#include <iostream>
#include "client_utils.h"
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>  // For mkdir

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define CHUNK_SIZE 1024
#define MAX_RETRIES 5

void requestLogFile(int sock) {
    // Ensure logs directory exists
    const char* logsDir = "client/logs";
    mkdir(logsDir, 0777);

    send(sock, "LOG_REQUEST", strlen("LOG_REQUEST"), 0);
    std::cout << "📤 Sent log request to server...\n";

    std::ofstream logFile("client/logs/received_logs.txt", std::ios::binary);
    if (!logFile) {
        std::cerr << "❌ Error: Failed to create log file. Make sure 'client/logs/' exists.\n";
        return;
    }

    char buffer[CHUNK_SIZE];
    int bytesRead;
    while ((bytesRead = recv(sock, buffer, CHUNK_SIZE, 0)) > 0) {
        logFile.write(buffer, bytesRead);
    }

    logFile.close();
    std::cout << "✅ Log file received and saved in client/logs/received_logs.txt" << std::endl;
}

int main() {
    int sock;
    int attempt = 0;

    // Retry connection logic (Auto-Reconnect)
    while (attempt < MAX_RETRIES) {
        sock = ClientUtils::createSocket();
        if (sock < 0) {
            std::cerr << "❌ Socket creation failed. Exiting.\n";
            return -1;
        }

        if (ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
            std::cout << "✅ Connected to SecureLink Server!\n";
            break;
        }

        std::cerr << "⚠️ Connection attempt " << (attempt + 1) << " failed. Retrying in 2 seconds...\n";
        close(sock);
        sleep(2);
        attempt++;
    }

    if (attempt == MAX_RETRIES) {
        std::cerr << "❌ Unable to establish connection after multiple attempts. Exiting.\n";
        return -1;
    }

    // Authentication
    std::string username, password;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::string credentials = username + ":" + password;
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Server Response: " << response << std::endl;

    // Ask user if they want logs
    std::cout << "Do you want to retrieve logs? (yes/no): ";
    std::string choice;
    std::cin >> choice;

    if (choice == "yes") {
        requestLogFile(sock);
    }

    ClientUtils::closeSocket(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
