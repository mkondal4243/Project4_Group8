#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <cassert>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLineEdit username;
    QLineEdit password;
    QPushButton loginBtn;

    username.setText("");
    password.setText("");

    // Simulate logic: if either field is empty, disable login
    bool isDisabled = username.text().isEmpty() || password.text().isEmpty();
    loginBtn.setEnabled(!isDisabled);

    assert(!loginBtn.isEnabled()); // should be false (disabled)
    return 0;
}
