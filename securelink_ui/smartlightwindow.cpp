#include "smartlightwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QLinearGradient>
#include <QFrame>
#include <QDateTime>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QFileDialog>
#include <QTextStream>

SmartLightWindow::SmartLightWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(700, 500);
    this->setWindowTitle("SecureLink – Smart Light");

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#fffde7"));
    gradient.setColorAt(1.0, QColor("#ffffff"));
    palette.setBrush(QPalette::Window, gradient);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: white; border-radius: 12px;");
    central->setFixedSize(620, 420);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Smart Light Control");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #827717;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ccc;");
    layout->addWidget(line);

    statusLabel = new QLabel("Light is OFF");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 15px; color: #333;");
    layout->addWidget(statusLabel);

    toggleBtn = new QPushButton("Turn ON");
    toggleBtn->setFixedHeight(40);
    toggleBtn->setStyleSheet("QPushButton { background-color: #FBC02D; color: white; font-weight: bold; border-radius: 8px; } QPushButton:hover { background-color: #F9A825; }");
    layout->addWidget(toggleBtn);

    historyList = new QListWidget();
    historyList->setStyleSheet("background-color: #fff9c4; border: none; font-size: 13px; padding: 8px;");
    layout->addWidget(historyList);

    QPushButton *exportBtn = new QPushButton("Export Toggle History");
    exportBtn->setFixedHeight(30);
    exportBtn->setStyleSheet("QPushButton { background-color: #795548; color: white; border-radius: 6px; } QPushButton:hover { background-color: #5D4037; }");
    layout->addWidget(exportBtn);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setFixedHeight(30);
    backBtn->setStyleSheet("QPushButton { background-color: #BCAAA4; color: white; border-radius: 6px; } QPushButton:hover { background-color: #8D6E63; }");
    layout->addWidget(backBtn);

    connect(toggleBtn, &QPushButton::clicked, this, [=]() {
        isLightOn = !isLightOn;
        QString state = isLightOn ? "ON" : "OFF";
        statusLabel->setText("Light is " + state);
        toggleBtn->setText(isLightOn ? "Turn OFF" : "Turn ON");

        // Log with timestamp
        QString logEntry = QString("💡 Light turned %1 at %2").arg(state, QDateTime::currentDateTime().toString("hh:mm:ss AP"));
        QListWidgetItem *item = new QListWidgetItem(logEntry);
        historyList->addItem(item);
        historyList->scrollToBottom();

        // Flicker animation effect
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(statusLabel);
        statusLabel->setGraphicsEffect(effect);
        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(0.3);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(exportBtn, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getSaveFileName(this, "Export Light History", "LightHistory.txt", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                for (int i = 0; i < historyList->count(); ++i)
                    out << historyList->item(i)->text() << "\n";
                file.close();
            }
        }
    });

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        this->hide();
        QTimer::singleShot(250, [=]() {
            emit backToDashboardRequested();
        });
    });
}
