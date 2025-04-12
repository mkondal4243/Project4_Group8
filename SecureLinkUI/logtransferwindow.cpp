#include "logtransferwindow.h"
#include "../client/client_utils.h"
#include "../shared/data_packet.h"
#include "login_backend.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDebug>
#include <QCoreApplication>

LogTransferWindow::LogTransferWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Log Transfer Interface", this);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QLabel *subText = new QLabel("View motion/system logs received from the server.", this);
    subText->setStyleSheet("color: #bbb; font-size: 13px;");
    subText->setAlignment(Qt::AlignCenter);
    layout->addWidget(subText);

    logDisplay = new QTextEdit(this);
    logDisplay->setReadOnly(true);
    logDisplay->setStyleSheet(R"(
        QTextEdit {
            background-color: #1e1e1e;
            color: #ddd;
            font-family: Consolas;
            font-size: 13px;
            border: 1px solid #444;
            border-radius: 6px;
        }
    )");
    layout->addWidget(logDisplay);

    receiveButton = new QPushButton("Receive Logs", this);
    saveButton = new QPushButton("Save to File", this);

    receiveButton->setStyleSheet("background-color: #444; color: white; padding: 10px; border-radius: 8px;");
    saveButton->setStyleSheet("background-color: #444; color: white; padding: 10px; border-radius: 8px;");

    layout->addWidget(receiveButton);
    layout->addWidget(saveButton);

    timestampLabel = new QLabel("Last received: --", this);
    timestampLabel->setStyleSheet("color: #aaa; font-size: 12px;");
    timestampLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(timestampLabel);

    statusLabel = new QLabel("No logs received yet.", this);
    statusLabel->setStyleSheet("color: #aaa; font-size: 12px;");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    connect(receiveButton, &QPushButton::clicked, this, &LogTransferWindow::receiveLogs);
    connect(saveButton, &QPushButton::clicked, this, &LogTransferWindow::saveLogsToFile);

    setLayout(layout);
    setStyleSheet("background-color: #2b2b2b;");
}

void LogTransferWindow::receiveLogs()
{
    // ✅ Print client path
    qDebug() << "🛠️ CLIENT PATH:" << QCoreApplication::applicationDirPath();

    QString logs = QString::fromStdString(ClientUtils::sendLogRequest());
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (logs.isEmpty() || logs.startsWith("❌")) {
        logs = R"(🚨 Motion detected at Garage [2025-04-10 14:21]
🔐 Lock toggled by admin [2025-04-10 14:25]
💡 Light toggled in Living Room [2025-04-10 14:27]
🛡️ Server entered MONITORING mode)";
        logDisplay->setPlainText(logs);
        statusLabel->setText("⚠️ Server failed. Showing fallback sample logs.");
    } else {
        logDisplay->setPlainText(logs);
        statusLabel->setText("✅ Logs received and displayed.");
    }

    timestampLabel->setText("Last received: " + timestamp);
}

void LogTransferWindow::saveLogsToFile()
{
    QString logs = logDisplay->toPlainText();

    if (logs.isEmpty()) {
        QMessageBox::warning(this, "Warning", "There are no logs to save.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Logs", "securelink_logs.txt", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << logs;
            file.close();
            statusLabel->setText("✅ Logs saved to file.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to save the file.");
        }
    }
}
