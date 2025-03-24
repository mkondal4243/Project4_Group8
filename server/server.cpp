#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server_logger.h"
#include "server_state.h"

#define SERVER_PORT 9090
#define MAX_BACKLOG 5  // Maximum clients in queue
#define CHUNK_SIZE 1024  // File transfer chunk size

void sendLogFile(int client_socket) {
    std::ifstream logFile("server/server_log.txt", std::ios::binary);
    if (!logFile) {
        std::cerr << "❌ Error: Unable to open log file!" << std::endl;
        ServerLogger::logEvent("Error: Unable to open log file for transfer");
        return;
    }

    char buffer[CHUNK_SIZE];
    while (logFile.read(buffer, CHUNK_SIZE) || logFile.gcount() > 0) {
        send(client_socket, buffer, logFile.gcount(), 0);
    }

    logFile.close();
    std::cout << "✅ Log file sent to client" << std::endl;
    ServerLogger::logEvent("✅ Log file successfully sent to client");
}

int main() {
    ServerStateManager serverState;
    serverState.setState(ServerState::IDLE);

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "❌ Error: Socket creation failed!\n";
        ServerLogger::logEvent("Error: Socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "❌ Error: Binding failed!\n";
        ServerLogger::logEvent("Error: Binding failed");
        return -1;
    }

    // Start listening
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

        // Receive message
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead <= 0) {
            std::cerr << "❌ Error: Failed to read from client\n";
            ServerLogger::logEvent("Error: Failed to read from client");
        } else {
            buffer[bytesRead] = '\0';
            std::cout << "📩 Received: " << buffer << std::endl;
            ServerLogger::logEvent("📩 Received data from client: " + std::string(buffer));

            // Handle log file request
            if (strcmp(buffer, "LOG_REQUEST") == 0) {
                sendLogFile(client_socket);
            } else {
                // Send acknowledgment for authentication
                std::string response = "✅ Received credentials";
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
