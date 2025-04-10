#include "accesslogswidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>


AccessLogsWidget::AccessLogsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Access Log Records", this);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QLabel *subtitle = new QLabel("Shows login attempts stored in the system.", this);
    subtitle->setStyleSheet("color: #aaa; font-size: 13px;");
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    usernameFilter = new QLineEdit(this);
    usernameFilter->setPlaceholderText("🔍 Filter by username...");
    usernameFilter->setStyleSheet("padding: 6px; font-size: 14px;");
    connect(usernameFilter, &QLineEdit::textChanged, this, &AccessLogsWidget::filterLogs);
    layout->addWidget(usernameFilter);

    logTable = new QTableWidget(this);
    logTable->setColumnCount(3);
    logTable->setHorizontalHeaderLabels({"Username", "Status", "Timestamp"});
    logTable->horizontalHeaderItem(0)->setToolTip("User who tried to log in");
    logTable->horizontalHeaderItem(1)->setToolTip("Login result or activity");
    logTable->horizontalHeaderItem(2)->setToolTip("Exact time of attempt");
    logTable->horizontalHeader()->setStretchLastSection(true);
    logTable->setStyleSheet("background-color: #1e1e1e; color: white; border: 1px solid #444;");
    layout->addWidget(logTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    refreshButton = new QPushButton("Refresh Logs", this);
    refreshButton->setStyleSheet("background-color: #444; color: white; padding: 10px; border-radius: 6px;");
    connect(refreshButton, &QPushButton::clicked, this, &AccessLogsWidget::loadAccessLogs);
    btnLayout->addWidget(refreshButton);

    exportButton = new QPushButton("Export Logs", this);
    exportButton->setStyleSheet("background-color: #444; color: white; padding: 10px; border-radius: 6px;");
    connect(exportButton, &QPushButton::clicked, this, &AccessLogsWidget::exportLogsToFile);
    btnLayout->addWidget(exportButton);

    layout->addLayout(btnLayout);

    statusLabel = new QLabel("", this);
    statusLabel->setStyleSheet("color: #aaa;");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    setLayout(layout);
    setStyleSheet("background-color: #2b2b2b;");

    loadAccessLogs(); // Initial load
}

void AccessLogsWidget::loadAccessLogs()
{
    logTable->setRowCount(0);

    QString connName = "access_logs_conn";
    if (QSqlDatabase::contains(connName))
        QSqlDatabase::removeDatabase(connName);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    QString dbPath = QCoreApplication::applicationDirPath() + "/../../../../../securelink.db";
    db.setDatabaseName(dbPath);
    qDebug() << "Trying to open DB at:" << dbPath;

    if (!db.open()) {
        showFallbackLogs("❌ Failed to open DB. Showing fallback logs.");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT username, action, timestamp FROM access_logs ORDER BY id DESC")) {
        showFallbackLogs("❌ DB Query failed. Showing fallback logs.");
        return;
    }

    int row = 0;
    while (query.next()) {
        logTable->insertRow(row);
        logTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        logTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        logTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        row++;
    }

    db.close();
    statusLabel->setText(QString("✅ Loaded %1 access logs from database.").arg(row));
}

void AccessLogsWidget::showFallbackLogs(const QString& reason)
{
    logTable->setRowCount(0);

    QList<QStringList> forcedLogs = {
        {"admin", "Login Success", "2025-04-10 14:30"},
        {"user123", "Toggled Light", "2025-04-10 14:31"},
        {"john", "Garage Access", "2025-04-10 14:32"},
        {"mehak", "Motion Alert Received", "2025-04-10 14:33"},
        {"nonso", "Logged Out", "2025-04-10 14:34"},
        {"sujahat", "Log Downloaded", "2025-04-10 14:35"},
        {"guest", "Login Failed", "2025-04-10 14:36"},
        {"root", "Lockdown Triggered", "2025-04-10 14:37"}
    };

    for (const QStringList &log : forcedLogs) {
        int row = logTable->rowCount();
        logTable->insertRow(row);
        for (int col = 0; col < log.size(); ++col) {
            logTable->setItem(row, col, new QTableWidgetItem(log[col]));
        }
    }

    statusLabel->setText(reason + " ✅ Showing fallback data.");
}

void AccessLogsWidget::filterLogs()
{
    QString filterText = usernameFilter->text().trimmed();

    for (int i = 0; i < logTable->rowCount(); ++i) {
        bool match = logTable->item(i, 0)->text().contains(filterText, Qt::CaseInsensitive);
        logTable->setRowHidden(i, !match);
    }
}

void AccessLogsWidget::exportLogsToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Logs", "access_logs.txt", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (int row = 0; row < logTable->rowCount(); ++row) {
                QString user = logTable->item(row, 0)->text();
                QString status = logTable->item(row, 1)->text();
                QString timestamp = logTable->item(row, 2)->text();
                out << user << " | " << status << " | " << timestamp << "\n";
            }
            file.close();
            statusLabel->setText("✅ Logs exported to file.");
        } else {
            statusLabel->setText("❌ Failed to export.");
        }
    }
}
