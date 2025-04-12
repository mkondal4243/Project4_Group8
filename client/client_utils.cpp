#include "client_utils.h"
#include "../shared/data_packet.h"
#include "../shared/base64.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace ClientUtils {

    int createSocket() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "❌ Error: Socket creation failed!" << std::endl;
            return -1;
        }
        return sock;
    }

    bool connectToServer(int sock, const std::string& serverIP, int port) {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        inet_pton(AF_INET, serverIP.c_str(), &server_address.sin_addr);

        if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            std::cerr << "❌ Error: Connection to server failed!" << std::endl;
            return false;
        }
        return true;
    }

    bool sendMessage(int sock, const std::string& message, PacketType type) {
        DataPacket packet(type, "Client", "Server", message);
        std::string serialized = packet.serialize();
        std::cout << "📤 Sending Packet: " << serialized << std::endl;

        int totalSent = 0;
        int messageLen = serialized.length();

        while (totalSent < messageLen) {
            int sent = send(sock, serialized.c_str() + totalSent, messageLen - totalSent, 0);
            if (sent == -1) {
                std::cerr << "❌ Error: Message sending failed!" << std::endl;
                return false;
            }
            totalSent += sent;
        }

        std::cout << "✅ Message fully sent (" << totalSent << " bytes)\n";
        return true;
    }

    bool sendMessage(int sock, const std::string& message) {
        return sendMessage(sock, message, PacketType::AUTH_REQUEST);
    }

    std::string receiveMessage(int sock) {
    char buffer[16384] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        std::cerr << "❌ Error: Receiving message failed!" << std::endl;
        return "";
    }

    buffer[bytesRead] = '\0';
    std::string raw(buffer);

    // Try parsing as DataPacket JSON
    try {
        DataPacket packet = DataPacket::deserialize(raw);

        // ✅ Check if this packet was sent as LOG_DATA
        if (!packet.data.empty()) {
    return packet.data; // ✅ Accept all valid responses, not just LOG_DATA
} else {
    std::cerr << "⚠️ Packet received but data is empty.\n";
    return "❌ Empty response from server.";
}
    } catch (...) {
        std::cerr << "⚠️ Raw message is not valid JSON, showing raw content.\n";
        return raw;
    }
}


    void closeSocket(int sock) {
        close(sock);
    }

    std::string sendCameraRequest(const std::string &room) {
        std::cout << "📸 sendCameraRequest() called for room: " << room << std::endl;

        int sock = createSocket();
        if (sock >= 0) {
            std::cout << "📡 Connecting to server...\n";
            if (connectToServer(sock, "127.0.0.1", 9090)) {
                std::string command = "CAMERA:" + room;
                std::cout << "🛰️ Sending command: " << command << std::endl;

                if (!sendMessage(sock, command, PacketType::LOG_REQUEST)) {
                    std::cerr << "❌ Failed to send camera request.\n";
                    closeSocket(sock);
                    return "❌ Camera request failed.";
                }

                std::string response = receiveMessage(sock);
                std::cout << "📥 Camera response: " << response << std::endl;

                closeSocket(sock);
                return response;
            } else {
                std::cerr << "❌ Connection to server failed.\n";
                closeSocket(sock);
                return "❌ Could not connect to server.";
            }
        } else {
            return "❌ Could not create socket.";
        }
    }

    std::string sendLogRequest() {
        int sock = createSocket();
        if (sock < 0 || !connectToServer(sock, "127.0.0.1", 9090)) {
            return "❌ Failed to connect to server.";
        }

        if (!sendMessage(sock, "LOG_REQUEST", PacketType::LOG_REQUEST)) {
            closeSocket(sock);
            return "❌ Failed to send log request.";
        }

        std::string response = receiveMessage(sock);
        closeSocket(sock);
        return response;
    }

    std::string saveSnapshotToFile() {
        int sock = createSocket();
        if (sock >= 0 && connectToServer(sock, "127.0.0.1", 9090)) {
            sendMessage(sock, "REQUEST_SNAPSHOT", PacketType::LOG_REQUEST);

            std::string encoded = receiveMessage(sock);
            closeSocket(sock);

            if (encoded.rfind("ERROR", 0) == 0)
                return "❌ " + encoded;

            std::string decoded = base64_decode(encoded);
            std::ofstream out("snapshot_from_server.jpg", std::ios::binary);
            out.write(decoded.c_str(), decoded.size());
            out.close();

            return "📸 Snapshot saved as snapshot_from_server.jpg";
        }
        return "❌ Failed to request snapshot.";
    }
}
