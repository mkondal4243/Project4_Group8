#include "login_backend.h"
#include "client_utils.h"
#include "dashboardwindow.h"  // 👈 Add this include to control UI

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090

namespace LoginBackend {

static std::string lastLogData;
static std::string lastMotionAlert;

// ✅ Pass pointer to dashboard window to update status
bool authenticate(const QString& username, const QString& password, DashboardWindow* dashboardWindow) {
    dashboardWindow->updateServerStatus("🟡 Connecting to Server...", "orange");

    int sock = ClientUtils::createSocket();
    if (sock < 0) {
        dashboardWindow->updateServerStatus("🔴 Socket Error", "red");
        return false;
    }

    if (!ClientUtils::connectToServer(sock, SERVER_IP, SERVER_PORT)) {
        ClientUtils::closeSocket(sock);
        dashboardWindow->updateServerStatus("🔴 Server Disconnected", "red");
        return false;
    }

    std::string credentials = username.toStdString() + ":" + password.toStdString();
    ClientUtils::sendMessage(sock, credentials);

    std::string authResponse = ClientUtils::receiveMessage(sock);
    if (authResponse.find("AUTH_SUCCESS") == std::string::npos) {
        ClientUtils::closeSocket(sock);
        dashboardWindow->updateServerStatus("🔴 Login Failed", "red");
        return false;
    }

    dashboardWindow->updateServerStatus("🟢 Server Status: Monitoring", "lightgreen");

    std::string logPacket = ClientUtils::receiveMessage(sock);
    lastLogData = logPacket;

    std::string motionPacket = ClientUtils::receiveMessage(sock);
    lastMotionAlert = motionPacket;

    QDir().mkpath("logs");
    QFile file("logs/received_logs.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(lastLogData) << "\n\n"
            << QString::fromStdString(lastMotionAlert);
        file.close();
    }

    ClientUtils::closeSocket(sock);
    return true;
}

QString receiveLogsAndEvents() {
    QFile file("logs/received_logs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "❌ Failed to open log file.\n";
    }

    QTextStream in(&file);
    QString logs = in.readAll();
    file.close();
    return logs;
}

}
