#include <iostream>
#include "client_utils.h"
#include "client_auth.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock = ClientUtils::createSocket();
    if (sock < 0) return -1;

    if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) return -1;

    std::cout << "Connected to SecureLink Server!\n";

    std::string credentials = ClientAuth::getCredentials();
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Server Response: " << response << std::endl;

    ClientUtils::closeSocket(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
