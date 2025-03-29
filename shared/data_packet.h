#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

// Enum defining different types of packets for communication
enum class PacketType : uint8_t {
    AUTH_REQUEST = 1,  
    AUTH_RESPONSE = 2, 
    LOG_TRANSFER = 3,  
    MOTION_ALERT = 4   
};

// Structure defining the fixed packet header
struct PacketHeader {
    uint8_t type;    
    uint16_t length; 
};

// Class representing a generic network packet
class Packet {
public:
    // Constructor to initialize a packet with a specific type and payload
    Packet(PacketType type, const std::vector<uint8_t>& data);

    // Serializes the packet (header + payload) into a byte stream for transmission
    std::vector<uint8_t> serialize() const;

    // Deserializes a received byte stream back into a Packet object
    static Packet deserialize(const std::vector<uint8_t>& rawData);

    // Retrieves the type of the packet (e.g., AUTH_REQUEST, LOG_TRANSFER)
    PacketType getType() const;

    // Retrieves the payload data of the packet
    std::vector<uint8_t> getPayload() const;

private:
    PacketHeader header;        
    std::vector<uint8_t> payload; 
};

#endif
