#include "../client/client_utils.h"
#include "../shared/data_packet.h"  // Required for PacketType
#include <cassert>
#include <iostream>

int main() {
    int sock = ClientUtils::createSocket();
    assert(sock >= 0);

    bool connected = ClientUtils::connectToServer(sock, "127.0.0.1", 9090);
    assert(connected);

    bool sent = ClientUtils::sendMessage(sock, "LOG_REQUEST", PacketType::LOG_REQUEST);
    assert(sent);

    std::string logs = ClientUtils::receiveMessage(sock);
    std::cout << "Logs received:\n" << logs << std::endl;

    assert(!logs.empty());
    assert(logs.rfind("❌", 0) != 0);  // Should not start with error

    ClientUtils::closeSocket(sock);

    return 0;
}
