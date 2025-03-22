#include <iostream>
#include "client_utils.h"
#include <unistd.h>  // For sleep()
#include "client_auth.h"
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define MAX_RETRIES 5
int main() {
    int sock //= ClientUtils::createSocket();
   // if (sock < 0) return -1;
    int attempt = 0;

    // Attempt to connect multiple times before failing
    while (attempt < MAX_RETRIES) {
        sock = ClientUtils::createSocket();
        if (sock < 0) {
            std::cerr << "❌ Socket creation failed. Exiting.\n";
            return -1;
        }

        if (ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
            std::cout << "✅ Connected to SecureLink Server!\n";
            break;
        }
    /*if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) return -1;

    std::cout << "Connected to SecureLink Server!\n";*/
    
        std::cerr << "⚠️ Connection attempt " << (attempt + 1) << " failed. Retrying in 2 seconds...\n";
        close(sock);
        sleep(2);  // Wait before retrying
        attempt++;
    }
    if (attempt == MAX_RETRIES) {
        std::cerr << "❌ Unable to establish connection after multiple attempts. Exiting.\n";
        return -1;
    }
    // Proceed with authentication and message exchange
    std::string username, password;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::string credentials = username + ":" + password;
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    std::cout << "Server Response: " << response << std::endl;

    ClientUtils::closeSocket(sock);
    std::cout << "Connection closed.\n";

    return 0;
}
