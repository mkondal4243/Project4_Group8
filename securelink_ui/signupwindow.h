#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEnterEvent>

class SignupWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);

signals:
    void backToLogin();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QLabel *titleLabel;
    QLineEdit *usernameInput;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmInput;
    QPushButton *signupButton;
    QLabel *loginPrompt;
};

#endif // SIGNUPWINDOW_H
