#include "securelinkwindow.h"
#include "signupwindow.h"
#include "forgotpasswordwindow.h"
#include "dashboardwindow.h"

#include <QFont>
#include <QPalette>
#include <QEnterEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLinearGradient>
#include <QLabel>

SecureLinkWindow::SecureLinkWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(400, 500);
    this->setWindowTitle("SecureLink – Login");

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#0f2027"));
    gradient.setColorAt(0.5, QColor("#203a43"));
    gradient.setColorAt(1.0, QColor("#2c5364"));
    palette.setBrush(QPalette::Window, gradient);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QWidget *card = new QWidget(this);
    card->setFixedSize(300, 350);
    card->setStyleSheet("background-color: #ffffff; border-radius: 12px;");
    card->move((width() - card->width()) / 2, (height() - card->height()) / 2);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    titleLabel = new QLabel("🔒 SecureLink", card);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #00ccff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(card);
    usernameInput->setPlaceholderText("Username");
    usernameInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(card);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(passwordInput);

    loginButton = new QPushButton("Login", card);
    loginButton->setStyleSheet(
        "QPushButton { background-color: #00c6ff; color: white; border-radius: 6px; padding: 10px; font-weight: bold; }"
        "QPushButton:hover { background-color: #0072ff; }"
        );
    layout->addWidget(loginButton);

    forgotPassword = new QLabel("<a href=\"#\">Forgot Password?</a>", card);
    forgotPassword->setStyleSheet("color: #2c5364; font-size: 12px;");
    forgotPassword->setAlignment(Qt::AlignCenter);
    forgotPassword->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotPassword->setOpenExternalLinks(false);
    layout->addWidget(forgotPassword);

    signupPrompt = new QLabel("Don't have an account? <a href=\"#\">Sign up</a>", card);
    signupPrompt->setStyleSheet("color: #2c5364; font-size: 12px;");
    signupPrompt->setAlignment(Qt::AlignCenter);
    signupPrompt->setTextInteractionFlags(Qt::TextBrowserInteraction);
    signupPrompt->setOpenExternalLinks(false);
    layout->addWidget(signupPrompt);

    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(card);
    card->setGraphicsEffect(fadeEffect);

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnim->setDuration(800);
    fadeAnim->setStartValue(0.0);
    fadeAnim->setEndValue(1.0);

    QPropertyAnimation *slideAnim = new QPropertyAnimation(card, "pos");
    slideAnim->setDuration(800);
    slideAnim->setStartValue(QPoint(card->x(), card->y() - 50));
    slideAnim->setEndValue(card->pos());

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(fadeAnim);
    group->addAnimation(slideAnim);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    // ✅ Login Button → Go to Dashboard
    connect(loginButton, &QPushButton::clicked, this, [=]() {
        // ⚠️ You can later add real auth here (e.g., check DB)
        DashboardWindow *dashboard = new DashboardWindow();
        connect(dashboard, &DashboardWindow::logoutRequested, this, [=]() {
            this->show();               // Show login again
            dashboard->deleteLater();  // Optional cleanup
        });
        dashboard->show();
        this->hide(); // Hide login window
    });

    // ✅ Sign Up Link Logic
    connect(signupPrompt, &QLabel::linkActivated, this, [=]() {
        SignupWindow *signupWindow = new SignupWindow();
        signupWindow->show();
        this->hide();

        connect(signupWindow, &SignupWindow::backToLogin, this, [=]() {
            this->show();
            signupWindow->deleteLater();
        });
    });

    // ✅ Forgot Password Link Logic
    connect(forgotPassword, &QLabel::linkActivated, this, [=]() {
        ForgotPasswordWindow *forgotWindow = new ForgotPasswordWindow();
        forgotWindow->show();
        this->hide();

        connect(forgotWindow, &ForgotPasswordWindow::backToLogin, this, [=]() {
            this->show();
            forgotWindow->deleteLater();
        });
    });
}

void SecureLinkWindow::enterEvent(QEnterEvent *event) {
    if (!titleLabel) return;

    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #00ffff;");
    auto *glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(25);
    glow->setColor(QColor("#00ffff"));
    glow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(glow);

    QMainWindow::enterEvent(event);
}

void SecureLinkWindow::leaveEvent(QEvent *event) {
    if (!titleLabel) return;

    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #00ccff;");
    titleLabel->setGraphicsEffect(nullptr);

    QMainWindow::leaveEvent(event);
}
