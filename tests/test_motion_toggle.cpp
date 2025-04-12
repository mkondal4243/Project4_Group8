#include <cassert>
#include <string>
#include <iostream>
#include "../client/client_utils.h"

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    bool connected = ClientUtils::connectToServer(sock, "127.0.0.1", 9090);
    assert(connected);

    
    bool sent = ClientUtils::sendMessage(sock, "MOTION_TEST");
    assert(sent);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Motion Response: " << response << std::endl;

    // We assume server will respond with an error or unknown response
    assert(response.find("Unknown") != std::string::npos);

    std::cout << "[PASS] Motion test responded correctly.\n";
    ClientUtils::closeSocket(sock);
    return 0;
}
