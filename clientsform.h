#ifndef CLIENTSFORM_H
#define CLIENTSFORM_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class ClientsForm : public QMainWindow
{
    Q_OBJECT

public:
    ClientsForm(const QString& role, QWidget *parent = nullptr);
    ~ClientsForm();

private slots:
    void loadClients();
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onTableClicked(int row, int column);

private:
    void clearFields();

    QString userRole;
    QTableWidget* tableWidget;
    QLineEdit* txtName;
    QLineEdit* txtPhone;
    QLineEdit* txtEmail;
    QPushButton* btnAdd;
    QPushButton* btnUpdate;
    QPushButton* btnDelete;
    QLabel* label1;
    QLabel* label2;
    QLabel* label3;
};

#endif // CLIENTSFORM_H