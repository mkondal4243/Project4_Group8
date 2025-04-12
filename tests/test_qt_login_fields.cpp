#include <QApplication>
#include <QLineEdit>
#include <cassert>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLineEdit usernameField;
    usernameField.setPlaceholderText("Username");

    QLineEdit passwordField;
    passwordField.setPlaceholderText("Password");

    assert(usernameField.placeholderText() == "Username");
    assert(passwordField.placeholderText() == "Password");

    return 0;
}
