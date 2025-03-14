#include <iostream>
#include <cstring>      // For memset
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h>  // For sockaddr_in
#include <unistd.h>     // For close()

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in server_address;

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

    // Send test message
    std::string message = "Hello from SecureLink Client!";
    send(sock, message.c_str(), message.length(), 0);

    // Close socket
    close(sock);
    std::cout << "Connection closed.\n";

    return 0;
}