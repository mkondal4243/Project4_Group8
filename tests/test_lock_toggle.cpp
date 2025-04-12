#include "../client/client_utils.h"
#include <cassert>
#include <iostream>

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    if (!ClientUtils::connectToServer(sock, "127.0.0.1", 9090)) {
        std::cerr << "[FAIL] Connection to server failed.\n";
        return 1;
    }

    bool sent = ClientUtils::sendMessage(sock, "LOCK_TOGGLE");
    assert(sent);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Lock Toggle Response: " << response << std::endl;

    assert(response.find("LOCK_SUCCESS") != std::string::npos);
    std::cout << "[PASS] Lock toggle response verified.\n";
    ClientUtils::closeSocket(sock);
    return 0;
}
