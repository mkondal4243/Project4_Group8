#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length;
    char buffer[1024] = {0};

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
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Binding failed!\n";
        return -1;
    }

    // Listen for connections
    if (listen(server_socket, 3) < 0) {
        std::cerr << "Error: Listening failed!\n";
        return -1;
    }

    std::cout << "SecureLink Server is running on port " << SERVER_PORT << "...\n";

    // Accept client connection
    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
    if (client_socket < 0) {
        std::cerr << "Error: Client connection failed!\n";
        return -1;
    }

    std::cout << "Client connected!\n";

    // Receive message
    read(client_socket, buffer, 1024);
    std::cout << "Received: " << buffer << "\n";

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
