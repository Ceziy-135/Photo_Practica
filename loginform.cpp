#include "loginform.h"
#include "mainwindow.h"
#include "databasehelper.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QComboBox>

LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Авторизация - Фото Мастерская");
    setFixedSize(350, 220);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QLabel* lblTitle = new QLabel("Фото Мастерская");
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(lblTitle);

    QHBoxLayout* loginLayout = new QHBoxLayout();
    label1 = new QLabel("Логин:");
    label1->setFixedWidth(70);
    txtLogin = new QLineEdit();
    loginLayout->addWidget(label1);
    loginLayout->addWidget(txtLogin);
    mainLayout->addLayout(loginLayout);

    QHBoxLayout* passLayout = new QHBoxLayout();
    label2 = new QLabel("Пароль:");
    label2->setFixedWidth(70);
    txtPassword = new QLineEdit();
    txtPassword->setEchoMode(QLineEdit::Password);
    passLayout->addWidget(label2);
    passLayout->addWidget(txtPassword);
    mainLayout->addLayout(passLayout);

    chkShowPassword = new QCheckBox("Показать пароль");
    connect(chkShowPassword, &QCheckBox::toggled, this, &LoginForm::showPassword);
    mainLayout->addWidget(chkShowPassword);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLogin = new QPushButton("Войти");
    btnRegister = new QPushButton("Регистрация");

    // Исправлено: цвет текста чёрный (color: black)
    btnLogin->setStyleSheet("background-color: #3498db; color: black;");
    btnRegister->setStyleSheet("background-color: #2ecc71; color: black;");

    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnRegister);
    mainLayout->addLayout(btnLayout);

    connect(btnLogin, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
    connect(btnRegister, &QPushButton::clicked, this, &LoginForm::onRegisterClicked);
}

LoginForm::~LoginForm() {}

void LoginForm::showPassword(bool checked)
{
    txtPassword->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void LoginForm::onLoginClicked()
{
    QString login = txtLogin->text().trimmed();
    QString password = txtPassword->text();

    if (login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль!");
        return;
    }

    QSqlQuery q = DatabaseHelper::executeQuery(
        QString("SELECT * FROM users WHERE login = '%1' AND password_hash = '%2'").arg(login).arg(password));

    if (q.next())
    {
        QString role = q.value("role").toString();
        MainWindow* mainWin = new MainWindow(role);
        mainWin->show();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
    }
}

void LoginForm::onRegisterClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Регистрация");
    dialog.setFixedSize(300, 250);
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLineEdit* txtNewLogin = new QLineEdit();
    txtNewLogin->setPlaceholderText("Логин");
    QLineEdit* txtNewPassword = new QLineEdit();
    txtNewPassword->setEchoMode(QLineEdit::Password);
    txtNewPassword->setPlaceholderText("Пароль");
    QLineEdit* txtConfirm = new QLineEdit();
    txtConfirm->setEchoMode(QLineEdit::Password);
    txtConfirm->setPlaceholderText("Подтверждение");
    QComboBox* cmbRole = new QComboBox();
    cmbRole->addItems({"user", "admin"});

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnOk = new QPushButton("OK");
    QPushButton* btnCancel = new QPushButton("Отмена");
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    layout->addWidget(txtNewLogin);
    layout->addWidget(txtNewPassword);
    layout->addWidget(txtConfirm);
    layout->addWidget(cmbRole);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, [&]() {
        QString login = txtNewLogin->text().trimmed();
        QString pass = txtNewPassword->text();
        QString conf = txtConfirm->text();
        if (login.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Заполните поля!");
            return;
        }
        if (pass != conf) {
            QMessageBox::warning(&dialog, "Ошибка", "Пароли не совпадают!");
            return;
        }
        if (DatabaseHelper::addUser(login, pass, cmbRole->currentText()))
        {
            QMessageBox::information(&dialog, "Успех", "Регистрация успешна!");
            dialog.accept();
        }
        else
        {
            QMessageBox::warning(&dialog, "Ошибка", "Логин занят!");
        }
    });
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}