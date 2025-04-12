#include <iostream>
#include <fstream>
#include <sstream>
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
#include "camera_handler.h"
#include "../shared/data_packet.h"
#include "../shared/base64.h"

#define SERVER_PORT 9090
#define MAX_BACKLOG 5
#define LOG_FILE_PATH "server/server_log.txt"

std::string getTimestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

void appendToLogFile(const std::string &entry) {
    std::ofstream log(LOG_FILE_PATH, std::ios::app);
    if (log.is_open()) {
        log << entry << "\n";
        log.close();
    }
}

void sendLogFile(int client_socket) {
    std::ifstream logFile(LOG_FILE_PATH);
    if (!logFile.is_open()) {
        std::cerr << "❌ LOG FILE NOT FOUND at: " << LOG_FILE_PATH << std::endl;
        DataPacket errorPacket(PacketType::LOG_DATA, "server", "client", "❌ Failed to open log file.");
        std::string response = errorPacket.serialize();
        send(client_socket, response.c_str(), response.length(), 0);
        return;
    }

    std::ostringstream contentStream;
    contentStream << logFile.rdbuf();  // Read full content
    std::string content = contentStream.str();

    DataPacket logPacket(PacketType::LOG_DATA, "server", "client", content);
    std::string response = logPacket.serialize();
    send(client_socket, response.c_str(), response.length(), 0);
    ServerLogger::logEvent("✅ Real server_log.txt sent to client");
    logFile.close();
}

int main() {
    ServerStateManager serverState;
    serverState.setState(ServerState::IDLE);

    if (!initializeDatabase()) {
        std::cerr << "❌ Failed to initialize database. Exiting.\n";
        return -1;
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))) {
        std::cout << "🛠️ SERVER CURRENT DIR: " << cwd << std::endl;
    }

    std::ofstream initLog(LOG_FILE_PATH, std::ios::app);
    if (initLog.is_open()) {
        initLog << "[INIT] Server log initialized\n";
        initLog.close();
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
            else if (packet.packet_type == PacketType::LOG_REQUEST && packet.data == "LOG_REQUEST") {
                ServerLogger::logEvent("📤 Sending logs to client...");
                sendLogFile(client_socket);
            }
            else if (packet.packet_type == PacketType::LOG_REQUEST && packet.data == "REQUEST_SNAPSHOT") {
                std::ifstream file("server/garage.jpg", std::ios::binary);
                if (!file.is_open()) {
                    std::cerr << "❌ Failed to open garage.jpg\n";
                    DataPacket errorPkt(PacketType::FILE_TRANSFER, "server", packet.sender, "ERROR: File missing");
                    send(client_socket, errorPkt.serialize().c_str(), errorPkt.serialize().length(), 0);
                } else {
                    std::ostringstream oss;
                    oss << file.rdbuf();
                    std::string imageData = oss.str();
                    std::string base64Encoded = base64_encode(reinterpret_cast<const unsigned char*>(imageData.c_str()), imageData.size());
                    DataPacket filePacket(PacketType::FILE_TRANSFER, "server", packet.sender, base64Encoded);
                    send(client_socket, filePacket.serialize().c_str(), filePacket.serialize().length(), 0);
                    ServerLogger::logEvent("📸 Snapshot sent to client.");
                }
            }
            else if (packet.data == "LOCK_TOGGLE") {
                appendToLogFile("[LOCK] Front Door toggled at " + getTimestamp());
                logAccess(packet.sender, "Toggled Smart Lock");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "LOCK_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data.rfind("LIGHT_TOGGLE", 0) == 0) {
                appendToLogFile("[LIGHT] Light toggled at " + getTimestamp());
                logAccess(packet.sender, "Toggled Smart Light");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "LIGHT_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data == "GARAGE_TOGGLE") {
                appendToLogFile("[GARAGE] Garage toggled at " + getTimestamp());
                logAccess(packet.sender, "Toggled Garage Door");
                DataPacket response(PacketType::SECURITY_EVENT, "server", packet.sender, "GARAGE_SUCCESS");
                send(client_socket, response.serialize().c_str(), response.serialize().length(), 0);
            }
            else if (packet.data.rfind("CAMERA:", 0) == 0) {
                std::string room = packet.data.substr(7);
                std::string camResponse = "[📷] Camera feed active in " + room;
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
