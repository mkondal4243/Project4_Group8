#include "../shared/data_packet.h"
#include <cassert>
#include <string>

int main() {
    PacketType pt = PacketType::AUTH_REQUEST;
    std::string expected = "AUTH_REQUEST";

    assert(packetTypeToString(pt) == expected);
    return 0;
}
