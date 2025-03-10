#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>

#define SERVER_PORT 8080
#define MAX_ATTEMPTS 3 // Maximum number of login attempts
// Hardcoded usernames & passwords
std::unordered_map<std::string, std::string> userDB = {
    {"admin", "password123"},
    {"user1", "securepass"},
    {"guest", "guest123"}
};

bool authenticateUser(const std::string &credentials) {
    size_t separator = credentials.find(":");
    if (separator == std::string::npos) return false;//ensure the credentials contain a colon separator

    std::string username = credentials.substr(0, separator);
    std::string password = credentials.substr(separator + 1);
    
    // Check for empty username or password
    if (username.empty() || password.empty()) {
        return false;
    }
    // Check if username exists and password matches
    return userDB.find(username) != userDB.end() && userDB[username] == password;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[1024] = { 0 };

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error: Socket creation failed!\n";
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind socket to port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Binding failed!\n";
        return -1;
    }

    // Listen for connections
    if (listen(server_socket, 3) < 0) {
        std::cerr << "Error: Listening failed!\n";
        return -1;
    }

    std::cout << "SecureLink Server is running on port " << SERVER_PORT << "...\n";

    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
    //Added a counter to track login attempts for each session
    int attemptCount = 0;
    bool locked = false;

    while (attemptCount < MAX_ATTEMPTS && !locked) {
        memset(buffer, 0, sizeof(buffer));
        read(client_socket, buffer, 1024);
//wrapped the authentication logic in a loop so the server can handle multiple login attempts 
 //without closing the connection after the first failure
        if (authenticateUser(buffer)) {
            std::string response = "Authentication Successful!";
            send(client_socket, response.c_str(), response.length(), 0);
            break;
        }
        else {
            attemptCount++;
            std::string response = "Authentication Failed! Attempts left: " + std::to_string(MAX_ATTEMPTS - attemptCount);
            send(client_socket, response.c_str(), response.length(), 0);
//the server enters 'lockdown mode' after too many failed attempts, sending an alert and blocking further logins
            if (attemptCount >= MAX_ATTEMPTS) {
                std::string lockdownMsg = "Too many failed attempts! Server in lockdown mode.";
                send(client_socket, lockdownMsg.c_str(), lockdownMsg.length(), 0);
                locked = true;
                std::cerr << "Alert: Server locked due to excessive failed logins!\n";
            }
        }
    }

    /*read(client_socket, buffer, 1024);
    std::string response = authenticateUser(buffer) ? "Authentication Successful!" : "Authentication Failed!";
    send(client_socket, response.c_str(), response.length(), 0);*/

    close(client_socket);
    close(server_socket);

    return 0;
}
