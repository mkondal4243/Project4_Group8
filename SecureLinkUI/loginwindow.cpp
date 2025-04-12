#include "loginwindow.h"
#include "login_backend.h"
#include "mainwindow.h"
#include "dashboardwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QFrame>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowState(Qt::WindowFullScreen);
    QWidget *central = new QWidget(this);
    central->setObjectName("centralWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *header = new QLabel("🔐 SecureLink", this);
    header->setStyleSheet("font-size: 28px; font-weight: bold; color: gold; background-color: #1a1a1a; padding: 12px;");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QLabel *tagline = new QLabel("Your Digital Fortress", this);
    tagline->setStyleSheet("font-size: 22px; font-weight: bold; color: #d4af37;");
    QLabel *desc = new QLabel("Military-grade protection for your home & data.", this);
    desc->setStyleSheet("font-size: 14px; color: #ccc;");
    desc->setWordWrap(true);
    leftLayout->addStretch();
    leftLayout->addWidget(tagline);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(desc);
    leftLayout->addStretch();
    leftLayout->setAlignment(Qt::AlignLeft);
    leftLayout->setContentsMargins(80, 20, 40, 20);

    QVBoxLayout *formLayout = new QVBoxLayout();
    QFrame *loginBox = new QFrame(this);
    loginBox->setObjectName("loginBox");

    QVBoxLayout *boxLayout = new QVBoxLayout();

    QLabel *loginTitle = new QLabel("Login", this);
    loginTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: white;");
    loginTitle->setAlignment(Qt::AlignCenter);

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Username");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    QPushButton *forgotPasswordBtn = new QPushButton("Forgot Password?", this);
    QPushButton *signupBtn = new QPushButton("Sign Up", this);
    forgotPasswordBtn->setFlat(true);
    signupBtn->setFlat(true);

    QHBoxLayout *links = new QHBoxLayout();
    links->addWidget(forgotPasswordBtn);
    links->addStretch();
    links->addWidget(signupBtn);

    boxLayout->addWidget(loginTitle);
    boxLayout->addSpacing(10);
    boxLayout->addWidget(usernameField);
    boxLayout->addWidget(passwordField);
    boxLayout->addWidget(loginButton);
    boxLayout->addLayout(links);
    loginBox->setLayout(boxLayout);

    formLayout->addStretch();
    formLayout->addWidget(loginBox, 0, Qt::AlignCenter);
    formLayout->addStretch();
    formLayout->setContentsMargins(40, 20, 80, 20);

    contentLayout->addLayout(leftLayout, 1);
    contentLayout->addLayout(formLayout, 1);
    mainLayout->addLayout(contentLayout);

    QLabel *footer = new QLabel("© 2025 SecureLink Security Systems", this);
    footer->setStyleSheet("font-size: 12px; color: #777; background-color: #1a1a1a; padding: 8px;");
    footer->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footer);

    setCentralWidget(central);
    setWindowTitle("SecureLink - Login");

    setStyleSheet(R"(
        #centralWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                        stop:0 #121212, stop:0.5 #2a2a2a, stop:1 #3a2c1c);
        }
        QLineEdit {
            background-color: #f5f5f5;
            border-radius: 6px;
            padding: 10px;
            font-size: 14px;
            color: #000;
        }
        QLineEdit::placeholder {
            color: #999;
        }
        QPushButton {
            background-color: #b8860b;
            color: white;
            border-radius: 6px;
            padding: 10px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #d4af37;
        }
        QPushButton:flat {
            background-color: transparent;
            color: #ffd700;
            font-size: 12px;
            text-decoration: underline;
        }
        #loginBox {
            background-color: #2e2e2e;
            border-radius: 12px;
            padding: 30px;
            max-width: 400px;
            min-width: 300px;
        }
    )");

    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
    central->setGraphicsEffect(fadeEffect);
    QPropertyAnimation *fadeAnim = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnim->setDuration(1000);
    fadeAnim->setStartValue(0);
    fadeAnim->setEndValue(1);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::handleLogin()
{
    QString user = usernameField->text();
    QString pass = passwordField->text();

    // 🔁 Direct dashboard status update during login
    DashboardWindow* dashboardWindow = new DashboardWindow();

    bool ok = LoginBackend::authenticate(user, pass, dashboardWindow);

    if (ok) {
        MainWindow* mainApp = new MainWindow();
        mainApp->show();
        this->close();
        delete dashboardWindow;  // dashboardWindow was just for status updating
    } else {
        delete dashboardWindow;
        QMessageBox::critical(this, "Login Failed", "Invalid credentials or server is offline.");
    }
}

LoginWindow::~LoginWindow() {}
