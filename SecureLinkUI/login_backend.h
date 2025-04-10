#ifndef LOGIN_BACKEND_H
#define LOGIN_BACKEND_H

#include <QString>

class DashboardWindow;  // Forward declaration (don't include .h here)

namespace LoginBackend {
bool authenticate(const QString& username, const QString& password, DashboardWindow* dashboardWindow);
QString receiveLogsAndEvents();
}

#endif // LOGIN_BACKEND_H
