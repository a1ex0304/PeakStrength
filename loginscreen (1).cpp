/**
 * @file loginscreen.cpp
 * @brief Implementation of LoginScreen 
 * @author Alexander Vong & Kibi Paskaran
*/

#include "loginscreen.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QPixmap>
#include <QDateTime>

/**
 * @brief Constructor of LoginScreen
 * @param parent; The parent widget, default is nullptr
*/


LoginScreen::LoginScreen(QWidget *parent) : QWidget(parent), userDatabase(userDB) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

//Set up the logo when on the login screen
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap("./logo.png");
    logoPixmap = logoPixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

//Login Label which is the title
    QLabel *title = new QLabel("Login", this);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

//Username field
    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameInput = new QLineEdit(this);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameInput);

//Password field
    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordInput);

//Login and Create Account Buttons
    QPushButton *loginButton = new QPushButton("Login", this);
    QPushButton *createAccountButton = new QPushButton("Create Account", this);
    layout->addWidget(loginButton);
    layout->addWidget(createAccountButton);

//Connecting the buttons
    connect(loginButton, &QPushButton::clicked, this, &LoginScreen::handleLogin);
    connect(createAccountButton, &QPushButton::clicked, this, &LoginScreen::handleCreateAccount);

    setLayout(layout);
    setWindowTitle("PeakStrength - Login");
}

/**
 * @brief Handling Login Attempts
 * It will check the username and password and will activate loginSuccess() if correct credentials
*/

void LoginScreen::handleLogin() {
    QString username = usernameInput->text().trimmed();
    QString password = passwordInput->text().trimmed();

    if (userDatabase->validateUser(username, password)) {
        emit loginSuccess();
        userDatabase->addWorkout(username, "Login at " + QDateTime::currentDateTime().toString());
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

/**
 * @brief Handling account creation
*/

void LoginScreen::handleCreateAccount() {
    QString username = usernameInput->text().trimmed();
    QString password = passwordInput->text().trimmed();

    if (userDatabase->userExists(username)) {
        QMessageBox::warning(this, "Create Account", "Username already exists.");
        return;
    }

    if (userDatabase->createUser(username, password)) {
        QMessageBox::information(this, "Success", "Account created!");
        userDatabase->addWorkout(username, "Account created at " + QDateTime::currentDateTime().toString());
    } else {
        QMessageBox::critical(this, "Error", "Account creation failed.");
    }
}