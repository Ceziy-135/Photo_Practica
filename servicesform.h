#ifndef SERVICESFORM_H
#define SERVICESFORM_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class ServicesForm : public QMainWindow
{
    Q_OBJECT

public:
    ServicesForm(const QString& role, QWidget *parent = nullptr);
    ~ServicesForm();

private slots:
    void loadServices();
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onShowFixedPrice();
    void onTableClicked(int row, int column);

private:
    void clearFields();

    QString userRole;
    QTableWidget* tableWidget;
    QLineEdit* txtName;
    QLineEdit* txtPrice;
    QTextEdit* txtDescription;
    QPushButton* btnAdd;
    QPushButton* btnUpdate;
    QPushButton* btnDelete;
    QPushButton* btnShowFixedPrice;
};

#endif // SERVICESFORM_H