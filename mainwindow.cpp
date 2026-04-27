#include "mainwindow.h"
#include "clientsform.h"
#include "executorsform.h"
#include "servicesform.h"
#include "ordersform.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCoreApplication>

MainWindow::MainWindow(const QString& role, QWidget *parent)
    : QMainWindow(parent), userRole(role)
{
    setWindowTitle("Главное меню - Фото Мастерская");
    setFixedSize(350, 500);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    lblWelcome = new QLabel("Фото Мастерская");
    lblWelcome->setAlignment(Qt::AlignCenter);
    lblWelcome->setStyleSheet("font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(lblWelcome);

    btnClients = new QPushButton("Заказчики работ");
    btnExecutors = new QPushButton("Исполнители");
    btnServices = new QPushButton("Прайс работ");
    btnOrders = new QPushButton("Журнал заказов");
    btnHelp = new QPushButton("Справка");
    btnExit = new QPushButton("Выход");

    // Исправлено: цвет текста чёрный (color: black)
    QString btnStyle = "QPushButton { background-color: #3498db; color: black; font-size: 12px; border-radius: 5px; padding: 8px; }"
                       "QPushButton:hover { background-color: #2980b9; }";
    btnClients->setStyleSheet(btnStyle);
    btnExecutors->setStyleSheet(btnStyle);
    btnServices->setStyleSheet(btnStyle);
    btnOrders->setStyleSheet(btnStyle);
    btnHelp->setStyleSheet(btnStyle);
    // Кнопка "Выход" — красный фон, чёрный текст
    btnExit->setStyleSheet("QPushButton { background-color: #e74c3c; color: black; }");

    mainLayout->addWidget(btnClients);
    mainLayout->addWidget(btnExecutors);
    mainLayout->addWidget(btnServices);
    mainLayout->addWidget(btnOrders);
    mainLayout->addWidget(btnHelp);
    mainLayout->addWidget(btnExit);

    connect(btnClients, &QPushButton::clicked, this, &MainWindow::onClientsClicked);
    connect(btnExecutors, &QPushButton::clicked, this, &MainWindow::onExecutorsClicked);
    connect(btnServices, &QPushButton::clicked, this, &MainWindow::onServicesClicked);
    connect(btnOrders, &QPushButton::clicked, this, &MainWindow::onOrdersClicked);
    connect(btnHelp, &QPushButton::clicked, this, &MainWindow::onHelpClicked);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onClientsClicked()
{
    ClientsForm* f = new ClientsForm(userRole);
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}

void MainWindow::onExecutorsClicked()
{
    ExecutorsForm* f = new ExecutorsForm(userRole);
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}

void MainWindow::onServicesClicked()
{
    ServicesForm* f = new ServicesForm(userRole);
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}

void MainWindow::onOrdersClicked()
{
    OrdersForm* f = new OrdersForm();
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}

void MainWindow::onHelpClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Выберите раздел");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Yes)->setText("Справка");
    msgBox.button(QMessageBox::No)->setText("О программе");
    msgBox.button(QMessageBox::Cancel)->setText("Отмена");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) showHelp();
    else if (ret == QMessageBox::No) showAbout();
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, "Справка",
                             "ФОТО МАСТЕРСКАЯ - программа для управления заказами\n\n"
                             "Функционал:\n1. Управление заказчиками\n2. Управление исполнителями\n"
                             "3. Прайс услуг (300/100 руб.)\n4. Журнал заказов\n5. Авторизация\n\n"
                             "Статусы: 'Не выполнено' / 'Выполнено'\n"
                             "Администратор - полный доступ\nПользователь - просмотр и создание заказов\n\n"
                             "© 2026 Беренский Дмитрий");
}

void MainWindow::showAbout()
{
    QMessageBox::information(this, "О программе",
                             "Название: Фото Мастерская\nВерсия: 1.0\nРазработчик: Беренский Дмитрий\n"
                             "Технологии: C++ Qt, PostgreSQL\n© 2026");
}

void MainWindow::onExitClicked()
{
    if (QMessageBox::question(this, "Выход", "Вы уверены?") == QMessageBox::Yes)
        QCoreApplication::quit();
}