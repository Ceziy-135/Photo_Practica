#ifndef ORDERSFORM_H
#define ORDERSFORM_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QPushButton>

class OrdersForm : public QMainWindow
{
    Q_OBJECT

public:
    OrdersForm(QWidget *parent = nullptr);
    ~OrdersForm();

private slots:
    void loadOrders();
    void loadComboBoxes();
    void onAddClicked();
    void onUpdateClicked();
    void onTableClicked(int row, int column);

private:
    void clearFields();

    QTableWidget* tableWidget;
    QComboBox* cmbClient;
    QComboBox* cmbExecutor;
    QComboBox* cmbService;
    QComboBox* cmbStatus;
    QDateEdit* dtpOrderDate;
    QTextEdit* txtDescription;
    QPushButton* btnAdd;
    QPushButton* btnUpdate;
};

#endif // ORDERSFORM_H