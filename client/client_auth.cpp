#include "client_auth.h"
#include <iostream>

namespace ClientAuth {

    // Simple XOR encryption (for demo purposes)
    std::string encryptPassword(const std::string& password) {
        std::string encrypted = password;
        char key = 'K'; // Simple key
        for (char& c : encrypted) {
            c ^= key;
        }
        return encrypted;
    }

    std::string getCredentials() {
        std::string username, password;
        std::cout << "Enter Username: ";
        std::cin >> username;
        std::cout << "Enter Password: ";
        std::cin >> password;

        std::string encryptedPassword = encryptPassword(password);
        return username + ":" + encryptedPassword;
    }

}
