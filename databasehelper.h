#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>

class DatabaseHelper
{
public:
    static bool connect();
    static void disconnect();
    static bool isConnected();

    static QSqlQuery executeQuery(const QString& query);
    static bool executeNonQuery(const QString& query);
    static QSqlQueryModel* getModel(const QString& query);

    static bool userExists(const QString& login);
    static bool addUser(const QString& login, const QString& password, const QString& role);

private:
    static QSqlDatabase db;
};

#endif // DATABASEHELPER_H