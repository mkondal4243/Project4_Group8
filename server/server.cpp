#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <limits.h>

#include "server_logger.h"
#include "server_state.h"
#include "server_db.h"
#include "../shared/data_packet.h"
#include "camera_handler.h"

#define SERVER_PORT 9090
#define MAX_BACKLOG 5
#define LOG_FILE_PATH "server_log.txt"

// 📝 Utility to log entries into server_log.txt
void appendToLogFile(const std::string &entry) {
    std::ofstream log(LOG_FILE_PATH, std::ios::app);
    if (log.is_open()) {
        log << entry << "\n";
        log.close();
    }
}

// 📦 Send full server_log.txt to client
void sendLogFile(int client_socket) {
    std::ifstream logFile(LOG_FILE_PATH);
    if (!logFile.is_open()) {
        std::cerr << "❌ LOG FILE NOT FOUND at: " << LOG_FILE_PATH << std::endl;
        DataPacket errorPacket(PacketType::LOG_DATA, "server", "client", "❌ Failed to open log file.");
        std::string response = errorPacket.serialize();
        send(client_socket, response.c_str(), response.length(), 0);
        return;
    }

    std::string content((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
    std::cout << "📤 Sending log contents:\n" << content << std::endl;  // ✅ Print content

    DataPacket logPacket(PacketType::LOG_DATA, "server", "client", content);
    std::string response = logPacket.serialize();
    send(client_socket, response.c_str(), response.length(), 0);
    ServerLogger::logEvent("✅ Log file sent to client");
    logFile.close();
}

// 🚨 Simulate motion and send + log it
void simulateMotionDetection(int client_socket) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string motionAlert = "🚨 Motion detected in Garage!";

    DataPacket motionPacket(PacketType::SECURITY_EVENT, "server", "client", motionAlert);
    std::string response = motionPacket.serialize();
    send(client_socket, response.c_str(), response.length(), 0);

    ServerLogger::logEvent("🚨 Motion alert sent to client");
    appendToLogFile("[MOTION] " + motionAlert);
}

int main() {
    ServerStateManager serverState;
    serverState.setState(ServerState::IDLE);

    if (!initializeDatabase()) {
        std::cerr << "❌ Failed to initialize database. Exiting.\n";
        return -1;
    }

    // ✅ Print working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))) {
        std::cout << "🛠️ SERVER CURRENT DIR: " << cwd << std::endl;
    }

    int server_socket, client_socket;
    struct sockaddr_in server_address{}, client_address{};
    socklen_t client_length;
    char buffer[2048];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "❌ Error: Socket creation failed!\n";
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "❌ Error: Binding failed!\n";
        return -1;
    }

    if (listen(server_socket, MAX_BACKLOG) < 0) {
        std::cerr << "❌ Error: Listening failed!\n";
        return -1;
    }

    serverState.setState(ServerState::MONITORING);
    ServerLogger::logEvent("✅ Server started on port 9090");
    std::cout << "🚀 Server is running on port " << SERVER_PORT << "...\n";

    while (true) {
        client_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);

        if (client_socket < 0) {
            std::cerr << "❌ Error: Accepting client connection failed!\n";
            continue;
        }

        ServerLogger::logEvent("✅ Client connected");
        memset(buffer, 0, sizeof(buffer));

        int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cerr << "❌ Error reading from client.\n";
            ServerLogger::logEvent("❌ Client read failed");
            close(client_socket);
            continue;
        }

        buffer[bytesRead] = '\0';
        std::string jsonStr(buffer);
        std::cout << "📦 Received JSON: " << jsonStr << std::endl;

        try {
            DataPacket packet = DataPacket::deserialize(jsonStr);
            ServerLogger::logEvent("📩 Packet received: " + packet.data);

            if (packet.packet_type == PacketType::AUTH_REQUEST) {
                DataPacket response(PacketType::AUTH_RESPONSE, "server", packet.sender, "AUTH_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
                logAccess(packet.sender, "Logged In");
            } 

            else if (packet.packet_type == PacketType::LOG_REQUEST) {
                ServerLogger::logEvent("📤 Sending logs to client...");
                sendLogFile(client_socket);
            }

            else if (packet.data == "LOCK_TOGGLE") {
                appendToLogFile("[LOCK] Front Door toggled at " + std::to_string(time(nullptr)));
                logAccess(packet.sender, "Toggled Smart Lock");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "LOCK_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data.rfind("LIGHT_TOGGLE", 0) == 0) {
                appendToLogFile("[LIGHT] Light toggled at " + std::to_string(time(nullptr)));
                logAccess(packet.sender, "Toggled Smart Light");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "LIGHT_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data == "GARAGE_TOGGLE") {
                appendToLogFile("[GARAGE] Garage toggled at " + std::to_string(time(nullptr)));
                logAccess(packet.sender, "Toggled Garage Door");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "GARAGE_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data.rfind("CAMERA:", 0) == 0) {
                std::string room = packet.data.substr(7);
                std::string camResponse = handleCameraRequest(room);
                DataPacket response(PacketType::LOG_DATA, "server", packet.sender, camResponse);
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else {
                DataPacket response(PacketType::UNKNOWN, "server", packet.sender, "❌ Unknown request");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
        } catch (const std::exception& ex) {
            std::cerr << "❌ JSON parsing failed: " << ex.what() << "\n";
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
