#include <cassert>
#include <vector>
#include <string>

std::vector<std::string> mockLogs() {
    return {"Client connected", "Motion detected", "Log saved"};
}

int main() {
    auto logs = mockLogs();
    assert(!logs.empty());
    assert(logs[0] == "Client connected");
    assert(logs[2].find("Log") != std::string::npos);
    return 0;
}
