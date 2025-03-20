#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <string>

namespace ClientUtils {
    int createSocket();
    bool connectToServer(int sock, const std::string& serverIP, int port);
    bool sendMessage(int sock, const std::string& message);
    std::string receiveMessage(int sock);
    void closeSocket(int sock);
}

#endif // CLIENT_UTILS_H


