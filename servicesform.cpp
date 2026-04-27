#include "servicesform.h"
#include "databasehelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QTextEdit>

ServicesForm::ServicesForm(const QString& role, QWidget *parent)
    : QMainWindow(parent), userRole(role)
{
    setWindowTitle("Прайс услуг - Фото Мастерская");
    setMinimumSize(900, 550);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    tableWidget = new QTableWidget();
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(tableWidget, &QTableWidget::cellClicked, this, &ServicesForm::onTableClicked);
    mainLayout->addWidget(tableWidget);

    QGridLayout* formLayout = new QGridLayout();
    QLabel* label1 = new QLabel("Наименование:");
    txtName = new QLineEdit();
    QLabel* label2 = new QLabel("Цена (руб.):");
    txtPrice = new QLineEdit();
    QLabel* label3 = new QLabel("Описание:");
    txtDescription = new QTextEdit();
    txtDescription->setMaximumHeight(80);
    formLayout->addWidget(label1, 0, 0);
    formLayout->addWidget(txtName, 0, 1);
    formLayout->addWidget(label2, 1, 0);
    formLayout->addWidget(txtPrice, 1, 1);
    formLayout->addWidget(label3, 2, 0);
    formLayout->addWidget(txtDescription, 2, 1);
    mainLayout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Добавить");
    btnUpdate = new QPushButton("Изменить");
    btnDelete = new QPushButton("Удалить");
    btnShowFixedPrice = new QPushButton("Фиксированный прайс");

    // Исправлено: цвет текста чёрный (color: black)
    btnAdd->setStyleSheet("background-color: #2ecc71; color: black;");
    btnUpdate->setStyleSheet("background-color: #3498db; color: black;");
    btnDelete->setStyleSheet("background-color: #e74c3c; color: black;");
    btnShowFixedPrice->setStyleSheet("background-color: #f39c12; color: black;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnDelete);
    btnLayout->addWidget(btnShowFixedPrice);
    mainLayout->addLayout(btnLayout);

    connect(btnAdd, &QPushButton::clicked, this, &ServicesForm::onAddClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &ServicesForm::onUpdateClicked);
    connect(btnDelete, &QPushButton::clicked, this, &ServicesForm::onDeleteClicked);
    connect(btnShowFixedPrice, &QPushButton::clicked, this, &ServicesForm::onShowFixedPrice);

    if (userRole != "admin")
    {
        btnAdd->setVisible(false);
        btnUpdate->setVisible(false);
        btnDelete->setVisible(false);
    }

    loadServices();
}

ServicesForm::~ServicesForm() {}

void ServicesForm::loadServices()
{
    QSqlQuery q = DatabaseHelper::executeQuery("SELECT id, name, price, description FROM services ORDER BY price");
    tableWidget->clear();
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"ID", "Услуга", "Цена (руб.)", "Описание"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    while (q.next())
    {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(q.value("id").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(q.value("name").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(q.value("price").toDouble(), 'f', 2)));
        tableWidget->setItem(row, 3, new QTableWidgetItem(q.value("description").toString()));
        row++;
    }
}

void ServicesForm::onAddClicked()
{
    if (txtName->text().trimmed().isEmpty() || txtPrice->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Заполните название и цену!");
        return;
    }
    bool ok;
    double price = txtPrice->text().toDouble(&ok);
    if (!ok || price < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Некорректная цена!");
        return;
    }
    if (price != 300 && price != 100)
    {
        if (QMessageBox::question(this, "Фиксированный прайс",
                                  "Вы вводите цену " + QString::number(price) + " руб.\n"
                                                                                "Фиксированные цены: 300 и 100 руб.\nПродолжить?",
                                  QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
            return;
    }
    QString query = QString("INSERT INTO services (name, price, description) VALUES ('%1', %2, '%3')")
                        .arg(txtName->text().trimmed())
                        .arg(price)
                        .arg(txtDescription->toPlainText().trimmed());
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Услуга добавлена!");
        loadServices();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить!");
}

void ServicesForm::onUpdateClicked()
{
    int row = tableWidget->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите услугу!");
        return;
    }
    int id = tableWidget->item(row, 0)->text().toInt();
    bool ok;
    double price = txtPrice->text().toDouble(&ok);
    if (!ok || price < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Некорректная цена!");
        return;
    }
    QString query = QString("UPDATE services SET name='%1', price=%2, description='%3' WHERE id=%4")
                        .arg(txtName->text().trimmed())
                        .arg(price)
                        .arg(txtDescription->toPlainText().trimmed())
                        .arg(id);
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Данные обновлены!");
        loadServices();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить!");
}

void ServicesForm::onDeleteClicked()
{
    int row = tableWidget->currentRow();
    if (row < 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить выбранную услугу?") != QMessageBox::Yes)
        return;
    int id = tableWidget->item(row, 0)->text().toInt();
    if (DatabaseHelper::executeNonQuery(QString("DELETE FROM services WHERE id=%1").arg(id)))
    {
        QMessageBox::information(this, "Успех", "Услуга удалена!");
        loadServices();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить!");
}

void ServicesForm::onShowFixedPrice()
{
    QMessageBox::information(this, "Фиксированный прайс",
                             "Съёмка + редакция фото – 300 руб.\n"
                             "Редактирование фото – 100 руб.");
}

void ServicesForm::onTableClicked(int row, int)
{
    txtName->setText(tableWidget->item(row, 1)->text());
    txtPrice->setText(tableWidget->item(row, 2)->text());
    txtDescription->setText(tableWidget->item(row, 3)->text());
}

void ServicesForm::clearFields()
{
    txtName->clear();
    txtPrice->clear();
    txtDescription->clear();
}