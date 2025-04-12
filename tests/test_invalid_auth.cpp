#include "../client/client_utils.h"
#include <cassert>
#include <iostream>

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    if (!ClientUtils::connectToServer(sock, "127.0.0.1", 9090)) {
        std::cerr << "[FAIL] Connection failed.\n";
        return 1;
    }

    bool sent = ClientUtils::sendMessage(sock, "wronguser:wrongpass");
    assert(sent);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Invalid Auth Response: " << response << std::endl;

    assert(response != "AUTH_SUCCESS");
    std::cout << "[PASS] Invalid login correctly rejected.\n";
    ClientUtils::closeSocket(sock);
    return 0;
}
