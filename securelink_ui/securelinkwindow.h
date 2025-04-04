#ifndef SECURELINKWINDOW_H
#define SECURELINKWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEnterEvent>

class SecureLinkWindow : public QMainWindow {
    Q_OBJECT

public:
    SecureLinkWindow(QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QLabel *titleLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QLabel *forgotPassword;
    QLabel *signupPrompt;
};

#endif // SECURELINKWINDOW_H
