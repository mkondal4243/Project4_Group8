#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <string>
#include <ctime>
#include <nlohmann/json.hpp> // JSON library for serialization

// Enum for different packet types
enum class PacketType {
    AUTH_REQUEST,
    AUTH_RESPONSE,
    LOG_DATA,
    SECURITY_EVENT,
    UNKNOWN
};

// DataPacket structure to represent a network message
struct DataPacket {
    PacketType packet_type;  // Type of packet
    std::time_t timestamp;   // Timestamp of packet creation
    std::string sender;      // Sender ID (client/server)
    std::string receiver;    // Receiver ID (client/server)
    std::string data;        // Data payload (log message, credentials, etc.)

    // Constructor to initialize a data packet
    DataPacket(PacketType type, const std::string& snd, const std::string& rcv, const std::string& dat);

    // Serialize DataPacket to JSON format
    std::string serialize() const;

    // Deserialize JSON string to DataPacket object
    static DataPacket deserialize(const std::string& json_str);

    // Convert PacketType to string (for JSON serialization)
    static std::string packetTypeToString(PacketType type);

    // Convert string to PacketType (for JSON deserialization)
    static PacketType stringToPacketType(const std::string& type_str);
};

#endif // DATA_PACKET_H
