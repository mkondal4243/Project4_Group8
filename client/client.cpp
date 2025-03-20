#include <iostream>
#include "client_utils.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090

int main() {
    int sock = ClientUtils::createSocket();
    if (sock < 0) return -1;

    if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) return -1;

    std::cout << "Connected to SecureLink Server!\n";

    std::string username, password;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::string credentials = username + ":" + password;
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Server Response: " << response << std::endl;

    ClientUtils::closeSocket(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
