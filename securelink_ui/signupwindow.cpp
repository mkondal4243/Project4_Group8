#include "signupwindow.h"
#include "securelinkwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPalette>
#include <QLinearGradient>

SignupWindow::SignupWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(400, 550);
    this->setWindowTitle("SecureLink – Sign Up");

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#0f2027"));
    gradient.setColorAt(0.5, QColor("#203a43"));
    gradient.setColorAt(1.0, QColor("#2c5364"));
    palette.setBrush(QPalette::Window, gradient);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QWidget *card = new QWidget(this);
    card->setFixedSize(300, 420);
    card->setStyleSheet("background-color: #ffffff; border-radius: 12px;");
    card->move((width() - card->width()) / 2, (height() - card->height()) / 2);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    titleLabel = new QLabel("🔐 Create Account", card);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #00ccff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(card);
    usernameInput->setPlaceholderText("Username");
    usernameInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(usernameInput);

    emailInput = new QLineEdit(card);
    emailInput->setPlaceholderText("Email");
    emailInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(emailInput);

    passwordInput = new QLineEdit(card);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(passwordInput);

    confirmInput = new QLineEdit(card);
    confirmInput->setPlaceholderText("Confirm Password");
    confirmInput->setEchoMode(QLineEdit::Password);
    confirmInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 6px; color: black;");
    layout->addWidget(confirmInput);

    signupButton = new QPushButton("Sign Up", card);
    signupButton->setStyleSheet(
        "QPushButton { background-color: #00c6ff; color: white; border-radius: 6px; padding: 10px; font-weight: bold; color: black; }"
        "QPushButton:hover { background-color: #0072ff; }");
    layout->addWidget(signupButton);

    loginPrompt = new QLabel("Already have an account? <a href=\"#\">Login</a>", card);
    loginPrompt->setStyleSheet("color: #2c5364; font-size: 12px;");
    loginPrompt->setAlignment(Qt::AlignCenter);
    loginPrompt->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout->addWidget(loginPrompt);

    connect(loginPrompt, &QLabel::linkActivated, this, [=]() {
        emit backToLogin();
        this->close();
    });

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
}

void SignupWindow::enterEvent(QEnterEvent *event) {
    if (!titleLabel) return;
    titleLabel->setStyleSheet("color: #00ffff; font-weight: bold; font-size: 22px;");
    auto *glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(20);
    glow->setColor(Qt::cyan);
    glow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(glow);
    QMainWindow::enterEvent(event);
}

void SignupWindow::leaveEvent(QEvent *event) {
    if (!titleLabel) return;
    titleLabel->setStyleSheet("color: #00ccff; font-weight: bold; font-size: 20px;");
    titleLabel->setGraphicsEffect(nullptr);
    QMainWindow::leaveEvent(event);
}
