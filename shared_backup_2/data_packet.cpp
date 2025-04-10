#include "data_packet.h"
#include <iostream>

using json = nlohmann::json;

DataPacket::DataPacket(PacketType type, const std::string& snd, const std::string& rcv, const std::string& dat)
    : packet_type(type), sender(snd), receiver(rcv), data(dat) {
    timestamp = std::time(nullptr);
}

std::string DataPacket::packetTypeToString(PacketType type) {
    switch (type) {
    case PacketType::AUTH_REQUEST: return "AUTH_REQUEST";
    case PacketType::AUTH_RESPONSE: return "AUTH_RESPONSE";
    case PacketType::LOG_DATA: return "LOG_DATA";
    case PacketType::LOG_REQUEST: return "LOG_REQUEST"; 
    case PacketType::SECURITY_EVENT: return "SECURITY_EVENT";
    default: return "UNKNOWN";
    }
}

PacketType DataPacket::stringToPacketType(const std::string& type_str) {
    if (type_str == "AUTH_REQUEST") return PacketType::AUTH_REQUEST;
    if (type_str == "AUTH_RESPONSE") return PacketType::AUTH_RESPONSE;
    if (type_str == "LOG_DATA") return PacketType::LOG_DATA;
    if (type_str == "LOG_REQUEST") return PacketType::LOG_REQUEST;
    if (type_str == "SECURITY_EVENT") return PacketType::SECURITY_EVENT;
    return PacketType::UNKNOWN;
}

std::string DataPacket::serialize() const {
    json j;
    j["packet_type"] = packetTypeToString(packet_type);
    j["timestamp"] = timestamp;
    j["sender"] = sender;
    j["receiver"] = receiver;
    j["data"] = data;
    return j.dump();
}

DataPacket DataPacket::deserialize(const std::string& json_str) {
    json j = json::parse(json_str);
    PacketType type = stringToPacketType(j["packet_type"].get<std::string>());
    std::time_t timestamp = j["timestamp"].get<std::time_t>();
    std::string sender = j["sender"].get<std::string>();
    std::string receiver = j["receiver"].get<std::string>();
    std::string data = j["data"].get<std::string>();
    DataPacket packet(type, sender, receiver, data);
    packet.timestamp = timestamp;
    return packet;
}
