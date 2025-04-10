#include "garagedoorwindow.h"
#include "client_utils.h"
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QDebug>
#include <QLabel>

GarageDoorWindow::GarageDoorWindow(QWidget *parent)
    : QWidget(parent), isOpen(false), doorHeight(200)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *header = new QLabel("Garage Door Control", this);
    header->setStyleSheet("font-size: 24px; font-weight: bold; color: gold;");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLabel *desc = new QLabel("Click below to secure or leave the garage door open.", this);
    desc->setStyleSheet("font-size: 13px; color: #aaa;");
    desc->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(desc);

    // Garage outer container
    garageContainer = new QFrame(this);
    garageContainer->setFixedSize(350, 200);
    garageContainer->setStyleSheet("background-color: #ddd; border: 3px solid #888;");
    garageContainer->setLayout(new QStackedLayout());

    // 🚗 Realistic Car Widget
    carWidget = new QWidget(garageContainer);
    carWidget->setFixedSize(160, 70);
    carWidget->move(95, 120);
    carWidget->setStyleSheet("background-color: transparent;");
    carWidget->hide();

    QLabel *carBody = new QLabel(carWidget);
    carBody->setGeometry(0, 20, 160, 40);
    carBody->setStyleSheet("background-color: darkred; border-radius: 6px;");

    QLabel *window = new QLabel(carWidget);
    window->setGeometry(20, 5, 120, 20);
    window->setStyleSheet("background-color: lightblue; border-radius: 5px;");

    QLabel *wheel1 = new QLabel(carWidget);
    wheel1->setGeometry(20, 60, 20, 20);
    wheel1->setStyleSheet("background-color: black; border-radius: 10px;");

    QLabel *wheel2 = new QLabel(carWidget);
    wheel2->setGeometry(120, 60, 20, 20);
    wheel2->setStyleSheet("background-color: black; border-radius: 10px;");

    // 🚪 Garage Door
    garageDoor = new QFrame(garageContainer);
    garageDoor->setStyleSheet("background-color: #444;");
    garageDoor->setGeometry(0, 0, 350, doorHeight);
    garageDoor->raise(); // always above car

    mainLayout->addWidget(garageContainer, 0, Qt::AlignCenter);

    // 🔘 Button
    doorButton = new QPushButton("Leave garage open", this);
    doorButton->setStyleSheet("background-color: #333; color: white; padding: 8px 18px; border-radius: 10px;");
    doorButton->setCursor(Qt::PointingHandCursor);
    connect(doorButton, &QPushButton::clicked, this, &GarageDoorWindow::toggleDoor);
    mainLayout->addWidget(doorButton, 0, Qt::AlignCenter);

    // 🧾 Status Label
    statusLabel = new QLabel("Garage secured. Now you may rest easy.", this);
    statusLabel->setStyleSheet("font-size: 14px; color: lightgreen; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setStyleSheet("background-color: #2b2b2b;");

    // 🔁 Timer logic
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, [this]() {
        if (isOpen && doorHeight > 10) {
            doorHeight -= 5;
        } else if (!isOpen && doorHeight < 200) {
            doorHeight += 5;
        } else {
            animationTimer->stop();
            carWidget->setVisible(isOpen);
        }
        garageDoor->setGeometry(0, 0, 350, doorHeight);
    });
}

void GarageDoorWindow::toggleDoor()
{
    isOpen = !isOpen;

    int sock = ClientUtils::createSocket();
    if (sock >= 0 && ClientUtils::connectToServer(sock, "127.0.0.1", 9090)) {
        ClientUtils::sendMessage(sock, "GARAGE_TOGGLE");
        QString response = QString::fromStdString(ClientUtils::receiveMessage(sock));
        qDebug() << "🚪 Server Response:" << response;
        ClientUtils::closeSocket(sock);
    }

    animationTimer->start(10); // smooth
    if (isOpen) {
        doorButton->setText("Secure garage now");
        statusLabel->setText("Garage door is OPEN — anyone could walk in!");
        statusLabel->setStyleSheet("font-size: 14px; color: orange; font-weight: bold;");
    } else {
        doorButton->setText("Leave garage open");
        statusLabel->setText("Garage secured. Now you may rest easy.");
        statusLabel->setStyleSheet("font-size: 14px; color: lightgreen; font-weight: bold;");
        carWidget->hide();  // hide early
    }
}
