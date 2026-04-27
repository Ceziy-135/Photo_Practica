#include "clientsform.h"
#include "databasehelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>

ClientsForm::ClientsForm(const QString& role, QWidget *parent)
    : QMainWindow(parent), userRole(role)
{
    setWindowTitle("Заказчики работ - Фото Мастерская");
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    tableWidget = new QTableWidget();
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(tableWidget, &QTableWidget::cellClicked, this, &ClientsForm::onTableClicked);
    mainLayout->addWidget(tableWidget);

    QGridLayout* formLayout = new QGridLayout();
    label1 = new QLabel("ФИО:");
    txtName = new QLineEdit();
    label2 = new QLabel("Телефон:");
    txtPhone = new QLineEdit();
    label3 = new QLabel("Email:");
    txtEmail = new QLineEdit();

    formLayout->addWidget(label1, 0, 0);
    formLayout->addWidget(txtName, 0, 1);
    formLayout->addWidget(label2, 1, 0);
    formLayout->addWidget(txtPhone, 1, 1);
    formLayout->addWidget(label3, 2, 0);
    formLayout->addWidget(txtEmail, 2, 1);
    mainLayout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Добавить");
    btnUpdate = new QPushButton("Изменить");
    btnDelete = new QPushButton("Удалить");

    // Исправлено: цвет текста чёрный (color: black)
    btnAdd->setStyleSheet("background-color: #2ecc71; color: black;");
    btnUpdate->setStyleSheet("background-color: #3498db; color: black;");
    btnDelete->setStyleSheet("background-color: #e74c3c; color: black;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnDelete);
    mainLayout->addLayout(btnLayout);

    connect(btnAdd, &QPushButton::clicked, this, &ClientsForm::onAddClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &ClientsForm::onUpdateClicked);
    connect(btnDelete, &QPushButton::clicked, this, &ClientsForm::onDeleteClicked);

    if (userRole != "admin")
    {
        btnAdd->setVisible(false);
        btnUpdate->setVisible(false);
        btnDelete->setVisible(false);
    }

    loadClients();
}

ClientsForm::~ClientsForm() {}

void ClientsForm::loadClients()
{
    QSqlQuery q = DatabaseHelper::executeQuery("SELECT id, name, phone, email FROM clients ORDER BY id");
    tableWidget->clear();
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"ID", "ФИО", "Телефон", "Email"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    while (q.next())
    {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(q.value("id").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(q.value("name").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(q.value("phone").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(q.value("email").toString()));
        row++;
    }
}

void ClientsForm::onAddClicked()
{
    if (txtName->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите ФИО заказчика!");
        return;
    }
    QString query = QString("INSERT INTO clients (name, phone, email) VALUES ('%1', '%2', '%3')")
                        .arg(txtName->text().trimmed())
                        .arg(txtPhone->text().trimmed())
                        .arg(txtEmail->text().trimmed());
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Заказчик добавлен!");
        loadClients();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить заказчика!");
}

void ClientsForm::onUpdateClicked()
{
    int row = tableWidget->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите заказчика!");
        return;
    }
    int id = tableWidget->item(row, 0)->text().toInt();
    QString query = QString("UPDATE clients SET name='%1', phone='%2', email='%3' WHERE id=%4")
                        .arg(txtName->text().trimmed())
                        .arg(txtPhone->text().trimmed())
                        .arg(txtEmail->text().trimmed())
                        .arg(id);
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Данные обновлены!");
        loadClients();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные!");
}

void ClientsForm::onDeleteClicked()
{
    int row = tableWidget->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите заказчика!");
        return;
    }
    if (QMessageBox::question(this, "Подтверждение", "Удалить выбранного заказчика?") != QMessageBox::Yes)
        return;
    int id = tableWidget->item(row, 0)->text().toInt();
    if (DatabaseHelper::executeNonQuery(QString("DELETE FROM clients WHERE id=%1").arg(id)))
    {
        QMessageBox::information(this, "Успех", "Заказчик удалён!");
        loadClients();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить заказчика!");
}

void ClientsForm::onTableClicked(int row, int column)
{
    Q_UNUSED(column);
    txtName->setText(tableWidget->item(row, 1)->text());
    txtPhone->setText(tableWidget->item(row, 2)->text());
    txtEmail->setText(tableWidget->item(row, 3)->text());
}

void ClientsForm::clearFields()
{
    txtName->clear();
    txtPhone->clear();
    txtEmail->clear();
}