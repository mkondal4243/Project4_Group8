#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <string>
#include <ctime>
#include "nlohmann/json.hpp"

enum class PacketType {
    AUTH_REQUEST,
    AUTH_RESPONSE,
    LOG_DATA,
    LOG_REQUEST,
    SECURITY_EVENT,
    UNKNOWN
};

struct DataPacket {
    PacketType packet_type;
    std::time_t timestamp;
    std::string sender;
    std::string receiver;
    std::string data;

    DataPacket(PacketType type, const std::string& snd, const std::string& rcv, const std::string& dat);
    std::string serialize() const;
    static DataPacket deserialize(const std::string& json_str);
    static std::string packetTypeToString(PacketType type);
    static PacketType stringToPacketType(const std::string& type_str);
};

#endif // DATA_PACKET_H
