#include "../client/client_utils.h"
#include <cassert>
#include <iostream>

int main() {
    std::string response = ClientUtils::sendCameraRequest("Garage");
    std::cout << "Camera Response: " << response << std::endl;

    // Assert it returns a non-error, non-empty string
    assert(!response.empty() && response.find("❌") == std::string::npos);
    
    std::cout << "[PASS] Camera feed request successful." << std::endl;
    return 0;
}
