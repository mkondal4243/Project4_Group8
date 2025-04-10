#include "client_utils.h"
#include "../shared/data_packet.h"
#include <iostream>
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

    // 🔁 Add this overload to maintain backward compatibility
    bool sendMessage(int sock, const std::string& message) {
    return sendMessage(sock, message, PacketType::AUTH_REQUEST);
    }



    std::string receiveMessage(int sock) {
        char buffer[2048] = {0};
        int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            std::cerr << "❌ Error: Receiving message failed!" << std::endl;
            return "";
        }

        buffer[bytesRead] = '\0';
        std::string jsonStr(buffer);
        DataPacket packet = DataPacket::deserialize(jsonStr);
        return packet.data;
    }

    void closeSocket(int sock) {
        close(sock);
    }

    std::string sendCameraRequest(const std::string &room) {
    int sock = createSocket();
    if (sock >= 0 && connectToServer(sock, "127.0.0.1", 9090)) {
        std::string command = "CAMERA:" + room;
        sendMessage(sock, command, PacketType::LOG_REQUEST); 
        std::string response = receiveMessage(sock);
        closeSocket(sock);
        return response;
    }
    return "Failed to contact camera server.";
    }
 
    std::string sendLogRequest() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Error: Socket creation failed!" << std::endl;
        return "❌ Failed to connect.";
    }

    struct sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "❌ Error: Connection to server failed!" << std::endl;
        close(sock);
        return "❌ Connection failed.";
    }

    // ✅ Use correct PacketType here!
    bool success = ClientUtils::sendMessage(sock, "LOG_REQUEST", PacketType::LOG_REQUEST);
    if (!success) {
    close(sock);
    return "❌ Failed to send log request.";
    }


    char buffer[8192] = {0};
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        std::cerr << "❌ Error: Receiving message failed!" << std::endl;
        close(sock);
        return "❌ Failed to receive response.";
    }

    buffer[bytesReceived] = '\0';
    std::string responseStr(buffer);

    try {
        DataPacket response = DataPacket::deserialize(responseStr);
        if (response.packet_type == PacketType::LOG_DATA) {
            std::cout << "📥 Received logs successfully\n";
            return response.data;
        } else {
            std::cerr << "❌ Unexpected packet type\n";
            return "❌ Unexpected response.";
        }
    } catch (const std::exception& ex) {
        std::cerr << "❌ Error: Failed to parse response: " << ex.what() << std::endl;
        return "❌ Invalid response format.";
    }

    close(sock);
}




}
