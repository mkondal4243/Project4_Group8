#include "motionalertswindow.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

MotionAlertsWindow::MotionAlertsWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(700, 500);
    this->setWindowTitle("SecureLink – Motion Alerts");

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #f3f5ff; border-radius: 14px;");
    central->setFixedSize(620, 420);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel *title = new QLabel("Live Motion Alerts");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #1a237e;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #999;");
    layout->addWidget(line);

    alertList = new QListWidget();
    alertList->setStyleSheet(R"(
        QListWidget {
            background-color: #e8eaf6;
            border: none;
            padding: 10px;
            font-size: 13px;
        }
        QListWidget::item {
            color: #1a237e;
            padding: 4px;
        }
        QListWidget::item:selected {
            background-color: #c5cae9;
            border-radius: 4px;
        }
    )");
    layout->addWidget(alertList);

    QPushButton *exportBtn = new QPushButton("Export Motion Alerts");
    exportBtn->setStyleSheet("QPushButton { background-color: #3949ab; color: white; font-weight: 600; border-radius: 6px; height: 36px; } QPushButton:hover { background-color: #303f9f; }");
    layout->addWidget(exportBtn);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setStyleSheet("QPushButton { background-color: #9fa8da; color: white; font-weight: 500; border-radius: 6px; height: 32px; } QPushButton:hover { background-color: #7986cb; }");
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        this->hide();
        QTimer::singleShot(200, [=]() {
            emit backToDashboardRequested();
        });
    });

    connect(exportBtn, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getSaveFileName(this, "Export Motion Logs", "MotionAlerts.txt", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                for (int i = 0; i < alertList->count(); ++i) {
                    out << alertList->item(i)->text() << "\n";
                }
                file.close();
            }
        }
    });

    // 🕹 Simulate live motion alerts
    QTimer *simulator = new QTimer(this);
    connect(simulator, &QTimer::timeout, this, [=]() {
        QString time = QDateTime::currentDateTime().toString("hh:mm:ss AP");
        QString alert = "⚠️ Motion detected at " + time;

        QListWidgetItem *item = new QListWidgetItem();
        QLabel *label = new QLabel(alert);
        label->setStyleSheet("color: #d32f2f; font-size: 13px;");

        QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(label);
        fadeEffect->setOpacity(0.0);
        label->setGraphicsEffect(fadeEffect);

        alertList->addItem(item);
        alertList->setItemWidget(item, label);
        alertList->scrollToBottom();

        QPropertyAnimation *fadeIn = new QPropertyAnimation(fadeEffect, "opacity");
        fadeIn->setDuration(700);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

        if (alertList->count() > 8)
            alertList->takeItem(0);
    });

    simulator->start(5000); // every 5 seconds
}
