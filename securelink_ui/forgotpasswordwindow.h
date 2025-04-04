#ifndef FORGOTPASSWORDWINDOW_H
#define FORGOTPASSWORDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEnterEvent>

class ForgotPasswordWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ForgotPasswordWindow(QWidget *parent = nullptr);

signals:
    void backToLogin();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QLabel *titleLabel;
    QLineEdit *emailInput;
    QPushButton *sendButton;
    QLabel *loginPrompt;
};

#endif // FORGOTPASSWORDWINDOW_H
