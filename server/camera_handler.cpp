#include "camera_handler.h"

std::string handleCameraRequest(const std::string &roomName) {
    if (roomName == "Garage") return "Garage camera active.";
    if (roomName == "Living Room") return "Connected to Living Room.";
    if (roomName == "Front Door") return "Monitoring Front Door.";
    if (roomName == "Backyard") return "Backyard feed ready.";
    if (roomName == "Kitchen") return "Kitchen camera streaming.";
    if (roomName == "Hallway") return "Hallway cam on duty.";

    return "Unknown room.";
}
