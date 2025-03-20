#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server_logger.h"

#define SERVER_PORT 8080
#define MAX_ATTEMPTS 3

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[1024] = { 0 };

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error: Socket creation failed!\n";
        ServerLogger::logEvent("Error: Socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Binding failed!\n";
        ServerLogger::logEvent("Error: Binding failed");
        return -1;
    }

    if (listen(server_socket, 3) < 0) {
        std::cerr << "Error: Listening failed!\n";
        ServerLogger::logEvent("Error: Listening failed");
        return -1;
    }

    ServerLogger::logEvent("Server started and listening for connections");

    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);

    if (client_socket < 0) {
        std::cerr << "Error: Accepting client connection failed!\n";
        ServerLogger::logEvent("Error: Client connection failed");
        return -1;
    }

    ServerLogger::logEvent("Client connected");

    read(client_socket, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;
    ServerLogger::logEvent("Received data from client: " + std::string(buffer));

    close(client_socket);
    close(server_socket);
    ServerLogger::logEvent("Server shut down");

    return 0;
}
