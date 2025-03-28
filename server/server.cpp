#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "server_logger.h"
#include "server_state.h"

#define SERVER_PORT 9090
#define MAX_BACKLOG 5
#define LOG_FILE_PATH "server_log.txt"

void sendLogFile(int client_socket) {
    std::ifstream logFile(LOG_FILE_PATH);
    if (!logFile.is_open()) {
        std::string errorMsg = "Error: Could not open log file.";
        send(client_socket, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string content((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
    send(client_socket, content.c_str(), content.length(), 0);
    logFile.close();
    ServerLogger::logEvent("✅ Log file sent to client");
}

void simulateMotionDetection(int client_socket) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string motionAlert = "MOTION_DETECTED: Movement detected in Zone A!";
    send(client_socket, motionAlert.c_str(), motionAlert.length(), 0);
    ServerLogger::logEvent("🚨 Motion detected alert sent to client");
}

int main() {
    ServerStateManager serverState;
    serverState.setState(ServerState::IDLE);

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "❌ Error: Socket creation failed!\n";
        ServerLogger::logEvent("Error: Socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "❌ Error: Binding failed!\n";
        ServerLogger::logEvent("Error: Binding failed");
        return -1;
    }

    if (listen(server_socket, MAX_BACKLOG) < 0) {
        std::cerr << "❌ Error: Listening failed!\n";
        ServerLogger::logEvent("Error: Listening failed");
        return -1;
    }

    serverState.setState(ServerState::MONITORING);
    ServerLogger::logEvent("✅ Server started, current state: " + serverState.getStateName());
    std::cout << "🚀 Server is running on port " << SERVER_PORT << "...\n";

    while (true) {
        client_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);

        if (client_socket < 0) {
            std::cerr << "❌ Error: Accepting client connection failed!\n";
            ServerLogger::logEvent("Error: Client connection failed");
            continue;
        }

        ServerLogger::logEvent("✅ Client connected");

        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            std::cerr << "❌ Error: Failed to read from client\n";
            ServerLogger::logEvent("Error: Failed to read from client");
        } else {
            buffer[bytesRead] = '\0';
            std::string received(buffer);
            std::cout << "📩 Received: " << received << std::endl;
            ServerLogger::logEvent("📩 Received data from client: " + received);

            if (received.find(":") != std::string::npos) {
                std::string response = "AUTH_SUCCESS";
                send(client_socket, response.c_str(), response.length(), 0);

                sendLogFile(client_socket);
                simulateMotionDetection(client_socket);
            } else {
                std::string response = "❌ Invalid credentials format.";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        }

        close(client_socket);
        ServerLogger::logEvent("👋 Client disconnected");
    }

    close(server_socket);
    ServerLogger::logEvent("🔴 Server shut down");
    return 0;
}
