#include <cassert>
#include <string>
#include <iostream>
#include "../client/client_utils.h"

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    bool connected = ClientUtils::connectToServer(sock, "127.0.0.1", 9090);
    assert(connected);

    bool sent = ClientUtils::sendMessage(sock, "LIGHT_TOGGLE");
    assert(sent);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Light Toggle Response: " << response << std::endl;

    assert(response.find("LIGHT_SUCCESS") != std::string::npos);
    std::cout << "[PASS] Light toggle test passed.\n";

    ClientUtils::closeSocket(sock);
    return 0;
}
