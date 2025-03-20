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
            std::cerr << "Error: Socket creation failed!" << std::endl;
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
            std::cerr << "Error: Connection to server failed!" << std::endl;
            return false;
        }
        return true;
    }

    bool sendMessage(int sock, const std::string& message) {
        if (send(sock, message.c_str(), message.length(), 0) < 0) {
            std::cerr << "Error: Message sending failed!" << std::endl;
            return false;
        }
        return true;
    }

    std::string receiveMessage(int sock) {
        char buffer[1024] = {0};
        if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "Error: Receiving message failed!" << std::endl;
            return "";
        }
        return std::string(buffer);
    }

    void closeSocket(int sock) {
        close(sock);
    }

}

