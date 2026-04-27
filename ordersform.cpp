#include "ordersform.h"
#include "databasehelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QDateEdit>
#include <QTextEdit>

OrdersForm::OrdersForm(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Журнал заказов - Фото Мастерская");
    setMinimumSize(1000, 600);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    tableWidget = new QTableWidget();
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(tableWidget, &QTableWidget::cellClicked, this, &OrdersForm::onTableClicked);
    mainLayout->addWidget(tableWidget);

    QGridLayout* formLayout = new QGridLayout();
    QLabel* label1 = new QLabel("Клиент:");
    cmbClient = new QComboBox();
    QLabel* label2 = new QLabel("Исполнитель:");
    cmbExecutor = new QComboBox();
    QLabel* label3 = new QLabel("Услуга:");
    cmbService = new QComboBox();
    QLabel* label4 = new QLabel("Статус:");
    cmbStatus = new QComboBox();
    cmbStatus->addItems({"Не выполнено", "Выполнено"});
    QLabel* label5 = new QLabel("Дата заказа:");
    dtpOrderDate = new QDateEdit();
    dtpOrderDate->setDate(QDate::currentDate());
    dtpOrderDate->setCalendarPopup(true);
    QLabel* label6 = new QLabel("Описание:");
    txtDescription = new QTextEdit();
    txtDescription->setMaximumHeight(80);

    formLayout->addWidget(label1, 0, 0); formLayout->addWidget(cmbClient, 0, 1);
    formLayout->addWidget(label2, 1, 0); formLayout->addWidget(cmbExecutor, 1, 1);
    formLayout->addWidget(label3, 2, 0); formLayout->addWidget(cmbService, 2, 1);
    formLayout->addWidget(label4, 3, 0); formLayout->addWidget(cmbStatus, 3, 1);
    formLayout->addWidget(label5, 4, 0); formLayout->addWidget(dtpOrderDate, 4, 1);
    formLayout->addWidget(label6, 5, 0); formLayout->addWidget(txtDescription, 5, 1);
    mainLayout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Добавить");
    btnUpdate = new QPushButton("Изменить");

    // Исправлено: цвет текста чёрный (color: black)
    btnAdd->setStyleSheet("background-color: #2ecc71; color: black;");
    btnUpdate->setStyleSheet("background-color: #3498db; color: black;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnUpdate);
    mainLayout->addLayout(btnLayout);

    connect(btnAdd, &QPushButton::clicked, this, &OrdersForm::onAddClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &OrdersForm::onUpdateClicked);

    loadComboBoxes();
    loadOrders();
}

OrdersForm::~OrdersForm() {}

void OrdersForm::loadComboBoxes()
{
    // Clients
    QSqlQuery qc = DatabaseHelper::executeQuery("SELECT id, name FROM clients ORDER BY name");
    cmbClient->clear();
    while (qc.next())
        cmbClient->addItem(qc.value("name").toString(), qc.value("id").toInt());

    // Executors
    QSqlQuery qe = DatabaseHelper::executeQuery("SELECT id, name FROM executors ORDER BY name");
    cmbExecutor->clear();
    cmbExecutor->addItem("(Не выбран)", -1);
    while (qe.next())
        cmbExecutor->addItem(qe.value("name").toString(), qe.value("id").toInt());

    // Services
    QSqlQuery qs = DatabaseHelper::executeQuery("SELECT id, name, price FROM services ORDER BY name");
    cmbService->clear();
    while (qs.next())
        cmbService->addItem(qs.value("name").toString() + " - " +
                                QString::number(qs.value("price").toDouble(), 'f', 2) + " руб.",
                            qs.value("id").toInt());
}

void OrdersForm::loadOrders()
{
    QString query = "SELECT o.id, c.name as client_name, e.name as executor_name, "
                    "s.name as service_name, s.price, o.order_date, o.status, o.description "
                    "FROM orders o "
                    "LEFT JOIN clients c ON o.client_id = c.id "
                    "LEFT JOIN executors e ON o.executor_id = e.id "
                    "LEFT JOIN services s ON o.service_id = s.id "
                    "ORDER BY o.order_date DESC";
    QSqlQuery q = DatabaseHelper::executeQuery(query);

    tableWidget->clear();
    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({"ID","Клиент","Исполнитель","Услуга","Цена","Дата","Статус","Описание"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    while (q.next())
    {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(q.value("id").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(q.value("client_name").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(q.value("executor_name").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(q.value("service_name").toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(q.value("price").toDouble(), 'f', 2)));
        tableWidget->setItem(row, 5, new QTableWidgetItem(q.value("order_date").toDate().toString("dd.MM.yyyy")));
        tableWidget->setItem(row, 6, new QTableWidgetItem(q.value("status").toString()));
        tableWidget->setItem(row, 7, new QTableWidgetItem(q.value("description").toString()));
        row++;
    }
}

void OrdersForm::onAddClicked()
{
    if (cmbClient->currentData().toInt() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите клиента!");
        return;
    }
    if (cmbService->currentData().toInt() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите услугу!");
        return;
    }
    int execId = cmbExecutor->currentData().toInt();
    QString execStr = (execId == -1) ? "NULL" : QString::number(execId);
    QString query = QString("INSERT INTO orders (client_id, executor_id, service_id, order_date, status, description) "
                            "VALUES (%1, %2, %3, '%4', '%5', '%6')")
                        .arg(cmbClient->currentData().toInt())
                        .arg(execStr)
                        .arg(cmbService->currentData().toInt())
                        .arg(dtpOrderDate->date().toString("yyyy-MM-dd"))
                        .arg(cmbStatus->currentText())
                        .arg(txtDescription->toPlainText().trimmed());
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Заказ добавлен!");
        loadOrders();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить заказ!");
}

void OrdersForm::onUpdateClicked()
{
    int row = tableWidget->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите заказ!");
        return;
    }
    int id = tableWidget->item(row, 0)->text().toInt();
    int execId = cmbExecutor->currentData().toInt();
    QString execStr = (execId == -1) ? "NULL" : QString::number(execId);
    QString query = QString("UPDATE orders SET client_id=%1, executor_id=%2, service_id=%3, "
                            "order_date='%4', status='%5', description='%6' WHERE id=%7")
                        .arg(cmbClient->currentData().toInt())
                        .arg(execStr)
                        .arg(cmbService->currentData().toInt())
                        .arg(dtpOrderDate->date().toString("yyyy-MM-dd"))
                        .arg(cmbStatus->currentText())
                        .arg(txtDescription->toPlainText().trimmed())
                        .arg(id);
    if (DatabaseHelper::executeNonQuery(query))
    {
        QMessageBox::information(this, "Успех", "Заказ обновлён!");
        loadOrders();
        clearFields();
    }
    else
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить заказ!");
}

void OrdersForm::onTableClicked(int row, int)
{
    // клиент
    QString clientName = tableWidget->item(row, 1)->text();
    int idx = cmbClient->findText(clientName);
    if (idx >= 0) cmbClient->setCurrentIndex(idx);
    // исполнитель
    QString execName = tableWidget->item(row, 2)->text();
    idx = cmbExecutor->findText(execName);
    if (idx >= 0) cmbExecutor->setCurrentIndex(idx);
    // услуга
    QString serviceFull = tableWidget->item(row, 3)->text();
    for (int i = 0; i < cmbService->count(); ++i)
        if (cmbService->itemText(i).startsWith(serviceFull))
        { cmbService->setCurrentIndex(i); break; }
    // дата
    dtpOrderDate->setDate(QDate::fromString(tableWidget->item(row, 5)->text(), "dd.MM.yyyy"));
    // статус
    cmbStatus->setCurrentText(tableWidget->item(row, 6)->text());
    // описание
    txtDescription->setText(tableWidget->item(row, 7)->text());
}

void OrdersForm::clearFields()
{
    if (cmbClient->count()) cmbClient->setCurrentIndex(0);
    if (cmbExecutor->count()) cmbExecutor->setCurrentIndex(0);
    if (cmbService->count()) cmbService->setCurrentIndex(0);
    cmbStatus->setCurrentIndex(0);
    dtpOrderDate->setDate(QDate::currentDate());
    txtDescription->clear();
}