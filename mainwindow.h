#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& role, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClientsClicked();
    void onExecutorsClicked();
    void onServicesClicked();
    void onOrdersClicked();
    void onHelpClicked();
    void onExitClicked();

private:
    void showHelp();
    void showAbout();

    QString userRole;
    QPushButton* btnClients;
    QPushButton* btnExecutors;
    QPushButton* btnServices;
    QPushButton* btnOrders;
    QPushButton* btnHelp;
    QPushButton* btnExit;
    QLabel* lblWelcome;
};

#endif // MAINWINDOW_H