#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void showPassword(bool checked);

private:
    QLineEdit* txtLogin;
    QLineEdit* txtPassword;
    QPushButton* btnLogin;
    QPushButton* btnRegister;
    QCheckBox* chkShowPassword;
    QLabel* label1;
    QLabel* label2;
};

#endif // LOGINFORM_H