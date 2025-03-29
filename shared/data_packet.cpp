#include "data_packet.h"

// Constructor: Initializes a packet with the specified type and payload
Packet::Packet(PacketType type, const std::vector<uint8_t>& data) {
    header.type = static_cast<uint8_t>(type); // Set packet type
    header.length = static_cast<uint16_t>(data.size()); // Set payload length
    payload = data; // Copy payload data
}

// Constructor: Initializes a packet with the specified type and payload 
header.type = static_cast<uint8_t>(type); // Convert enum to uint8_t
header.length = static_cast<uint16_t>(data.size()); // Store payload size
payload = data; // Store payload data
}

// Serializes the packet into a byte vector for transmission
std::vector<uint8_t> Packet::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(sizeof(PacketHeader) + payload.size()); // Preallocate memory

    // Append header to buffer
    buffer.push_back(header.type);
    buffer.push_back(static_cast<uint8_t>(header.length & 0xFF)); // Lower byte of length
    buffer.push_back(static_cast<uint8_t>((header.length >> 8) & 0xFF)); // Upper byte of length

    // Append payload to buffer
    buffer.insert(buffer.end(), payload.begin(), payload.end());

    return buffer;
}

// Deserializes a byte vector into a Packet object
Packet Packet::deserialize(const std::vector<uint8_t>& rawData) {
    if (rawData.size() < sizeof(PacketHeader)) {
        throw std::runtime_error("Invalid packet data"); // Ensure valid packet size
    }

    PacketHeader header;
    header.type = rawData[0]; // Extract packet type
    header.length = static_cast<uint16_t>(rawData[1]) | (static_cast<uint16_t>(rawData[2]) << 8); // Extract length

    if (rawData.size() < sizeof(PacketHeader) + header.length) {
        throw std::runtime_error("Incomplete packet data"); // Ensure data integrity
    }

    // Extract payload data
    std::vector<uint8_t> payload(rawData.begin() + sizeof(PacketHeader), rawData.end());

    return Packet(static_cast<PacketType>(header.type), payload);
}

// Returns the type of the packet
PacketType Packet::getType() const {
    return static_cast<PacketType>(header.type);
}

//Retrieves the payload of the packet
std::vector<uint8_t> Packet::getPayload() const {
    return payload;
}
