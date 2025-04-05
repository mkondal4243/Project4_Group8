#include "login_backend.h"
#include "client_utils.h"
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090

namespace LoginBackend {

static std::string lastLogData;
static std::string lastMotionAlert;

bool authenticate(const QString& username, const QString& password) {
    int sock = ClientUtils::createSocket();
    if (sock < 0) return false;

    if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
        ClientUtils::closeSocket(sock);
        return false;
    }

    std::string credentials = username.toStdString() + ":" + password.toStdString();
    ClientUtils::sendMessage(sock, credentials);

    std::string response = ClientUtils::receiveMessage(sock);
    if (response == "AUTH_SUCCESS") {
        lastLogData = ClientUtils::receiveMessage(sock);
        lastMotionAlert = ClientUtils::receiveMessage(sock);
    }

    ClientUtils::closeSocket(sock);
    return response == "AUTH_SUCCESS";
}

QString receiveLogsAndEvents() {
    return QString::fromStdString(lastLogData + "\n\n" + lastMotionAlert);
}

}
