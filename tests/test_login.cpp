#include "../client/client_utils.h"
#include <cassert>
#include <iostream>

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    bool connected = ClientUtils::connectToServer(sock, "127.0.0.1", 9090);
    assert(connected);

    bool sent = ClientUtils::sendMessage(sock, "admin:password123");
    assert(sent);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << " Response from server: " << response << std::endl;

    assert(response == "AUTH_SUCCESS");

    ClientUtils::closeSocket(sock);

    return 0;
}
