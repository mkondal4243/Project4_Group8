#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error: Socket creation failed!\n";
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Connection to server failed!\n";
        return -1;
    }

    std::cout << "Connected to SecureLink Server!\n";

    // Get username and password input
    std::string username, password;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    // Send credentials to the server
    std::string credentials = username + ":" + password;
    send(sock, credentials.c_str(), credentials.length(), 0);

    // Wait for authentication response
    read(sock, buffer, 1024);
    std::cout << "Server Response: " << buffer << "\n";

    // Close socket
    close(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
