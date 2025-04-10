#ifndef CLIENT_AUTH_H
#define CLIENT_AUTH_H

#include <string>

namespace ClientAuth {
    std::string encryptPassword(const std::string& password);
    std::string getCredentials();
}

#endif // CLIENT_AUTH_H
