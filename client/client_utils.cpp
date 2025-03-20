#include "client_utils.h"
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

    bool sendMessage(int sock, const std::string& message) {
        int totalSent = 0;
        int messageLen = message.length();

        while (totalSent < messageLen) {
            int sent = send(sock, message.c_str() + totalSent, messageLen - totalSent, 0);
            if (sent == -1) {
                std::cerr << "❌ Error: Message sending failed!" << std::endl;
                return false;
            }
            totalSent += sent;
        }
        std::cout << "✅ Message fully sent (" << totalSent << " bytes)" << std::endl;
        return true;
    }

    std::string receiveMessage(int sock) {
        char buffer[1024] = {0};
        int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            std::cerr << "❌ Error: Receiving message failed!" << std::endl;
            return "";
        }

        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }

    void closeSocket(int sock) {
        close(sock);
    }

}
