#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>

// Function to print test results
void printTestResult(const std::string& testName, bool passed) {
    std::cout << "[" << (passed ? "PASS" : "FAIL") << "] " << testName << std::endl;
}

// Test server initialization
bool testServerInit() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bool result = (serverSocket >= 0);

    if (serverSocket >= 0) {
        close(serverSocket);
    }

    return result;
}

// Test server binding
bool testServerBind() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8081); // Use different port for testing

    // Allow port reuse for testing
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int bindResult = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    bool result = (bindResult >= 0);

    close(serverSocket);
    return result;
}

// Test server listening
bool testServerListen() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8082); // Use different port for testing

    // Allow port reuse for testing
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(serverSocket);
        return false;
    }

    int listenResult = listen(serverSocket, 3);
    bool result = (listenResult >= 0);

    close(serverSocket);
    return result;
}

// Test config file loading
bool testConfigLoading() {
    // Create a temporary test config file
    std::ofstream configFile("test_config.json");
    configFile << "{\n";
    configFile << "  \"network\": {\n";
    configFile << "    \"port\": 8080\n";
    configFile << "  }\n";
    configFile << "}\n";
    configFile.close();

    // Try to open and read the file
    std::ifstream inFile("test_config.json");
    bool result = inFile.good();

    // Clean up
    inFile.close();
    remove("test_config.json");

    return result;
}

int main() {
    std::cout << "=== SecureLink Server Tests ===" << std::endl;

    // Run tests
    std::vector<std::pair<std::string, bool>> testResults = {
        {"Server Socket Creation", testServerInit()},
        {"Server Socket Binding", testServerBind()},
        {"Server Socket Listening", testServerListen()},
        {"Config File Loading", testConfigLoading()}
    };

    // Output results
    int passed = 0;
    std::cout << "\nTest Results:\n";
    for (const auto& test : testResults) {
        printTestResult(test.first, test.second);
        if (test.second) passed++;
    }

    std::cout << "\nSummary: " << passed << "/" << testResults.size()
        << " tests passed." << std::endl;

    return (passed == testResults.size()) ? 0 : 1;
}