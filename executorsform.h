#ifndef EXECUTORSFORM_H
#define EXECUTORSFORM_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

class ExecutorsForm : public QMainWindow
{
    Q_OBJECT

public:
    ExecutorsForm(const QString& role, QWidget *parent = nullptr);
    ~ExecutorsForm();

private slots:
    void loadExecutors();
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
};

#endif // EXECUTORSFORM_H