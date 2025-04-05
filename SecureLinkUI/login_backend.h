#ifndef LOGIN_BACKEND_H
#define LOGIN_BACKEND_H

#include <QString>

namespace LoginBackend {
bool authenticate(const QString& username, const QString& password);
QString receiveLogsAndEvents();  // New function to receive log + motion alert
}

#endif // LOGIN_BACKEND_H
