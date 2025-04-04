#include "accesslogswindow.h"
#include "logtransferwindow.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

AccessLogsWindow::AccessLogsWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(680, 460);
    this->setWindowTitle("SecureLink – Access Logs");

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #fff8f0; border-radius: 12px;");
    central->setFixedSize(600, 400);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(18);

    QLabel *title = new QLabel("Access Logs");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #4e342e;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #bcaAA4;");
    layout->addWidget(line);

    QLabel *logs = new QLabel(
        "John accessed Main Door at 10:03 AM\n"
        "Alice accessed Garage at 11:15 AM\n"
        "Sam accessed Back Door at 12:20 PM\n"
        "Unknown access attempt at 01:03 PM"
        );
    logs->setStyleSheet("background-color: #fcefe8; padding: 12px; font-size: 13px; color: #3e2723; border-radius: 8px;");
    logs->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    logs->setMinimumHeight(140);
    logs->setWordWrap(true);
    layout->addWidget(logs);

    QPushButton *exportBtn = new QPushButton("Export Logs");
    exportBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #6d4c41;
            color: white;
            font-weight: bold;
            border-radius: 6px;
            height: 38px;
        }
        QPushButton:hover {
            background-color: #5d4037;
        }
    )");
    layout->addWidget(exportBtn);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #a1887f;
            color: white;
            border-radius: 6px;
            height: 32px;
        }
        QPushButton:hover {
            background-color: #8d6e63;
        }
    )");
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        this->hide();
        QTimer::singleShot(250, [=]() {
            emit backToDashboardRequested();
        });
    });

    connect(exportBtn, &QPushButton::clicked, this, [=]() {
        this->hide();
        QTimer::singleShot(200, [=]() {
            LogTransferWindow *logTransfer = new LogTransferWindow();
            connect(logTransfer, &LogTransferWindow::backToDashboardRequested, this, [=]() {
                emit backToDashboardRequested();
            });
            logTransfer->show();
        });
    });
}
