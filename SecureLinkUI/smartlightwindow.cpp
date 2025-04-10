#include "smartlightwindow.h"
#include "client_utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>

SmartLightWindow::SmartLightWindow(QWidget *parent)
    : QWidget(parent), lightOn(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *header = new QLabel("Smart Light Control", this);
    header->setStyleSheet("font-size: 24px; font-weight: bold; color: gold;");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLabel *desc = new QLabel("Control smart lights in different zones", this);
    desc->setStyleSheet("color: #aaa; font-size: 14px;");
    desc->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(desc);

    roomSelector = new QComboBox(this);
    roomSelector->addItems({"Living Room", "Kitchen", "Bedroom", "Garage"});
    roomSelector->setStyleSheet("padding: 8px; background: #444; color: white; font-size: 14px; border-radius: 6px;");
    mainLayout->addWidget(roomSelector, 0, Qt::AlignCenter);

    // Static bulb image label
    bulbLabel = new QLabel(this);
    bulbLabel->setFixedSize(180, 180);
    bulbLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(bulbLabel, 0, Qt::AlignCenter);

    // Toggle
    switchControl = new ToggleSwitch(this);
    connect(switchControl, &ToggleSwitch::toggled, this, &SmartLightWindow::toggleLight);
    mainLayout->addWidget(switchControl, 0, Qt::AlignCenter);

    // Status
    statusLabel = new QLabel("Light is off. Silence, darkness, and peace.", this);
    statusLabel->setStyleSheet("color: #aaa; font-size: 13px;");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setStyleSheet("background-color: #2b2b2b;");
}

void SmartLightWindow::toggleLight(bool checked)
{
    lightOn = checked;
    QString room = roomSelector->currentText();

    int sock = ClientUtils::createSocket();
    if (sock >= 0 && ClientUtils::connectToServer(sock, "127.0.0.1", 9090)) {
        QString command = QString("LIGHT_TOGGLE:%1").arg(room);
        ClientUtils::sendMessage(sock, command.toStdString());
        QString response = QString::fromStdString(ClientUtils::receiveMessage(sock));
        qDebug() << "Server Response:" << response;
        ClientUtils::closeSocket(sock);
    }

    if (lightOn) {
        bulbLabel->setPixmap(QPixmap(":/images/bulb_on.png").scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QString message;
        if (room == "Living Room")
            message = "Living Room is now bright enough to host a TED Talk.";
        else if (room == "Kitchen")
            message = "Kitchen lights are on — time to burn something delicious.";
        else if (room == "Bedroom")
            message = "Bedroom lighting set. You officially can't sleep now.";
        else if (room == "Garage")
            message = "Garage is lit. Your tools feel seen.";

        statusLabel->setText(message);
        statusLabel->setStyleSheet("color: #2e8b57; font-size: 13px;");
        this->setStyleSheet("background-color: #fdf6e3;");
    } else {
        bulbLabel->setPixmap(QPixmap(":/images/bulb_off.png").scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        statusLabel->setText("Light is off. Silence, darkness, and peace.");
        statusLabel->setStyleSheet("color: #aaa; font-size: 13px;");
        this->setStyleSheet("background-color: #2b2b2b;");
    }
}
