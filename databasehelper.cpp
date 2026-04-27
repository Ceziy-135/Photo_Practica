#include "databasehelper.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

QSqlDatabase DatabaseHelper::db;

bool DatabaseHelper::connect()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("PhotoMaster");
    db.setUserName("postgres");
    db.setPassword("postgres");   // ЗАМЕНИТЕ НА ВАШ ПАРОЛЬ

    if (!db.open())
    {
        qDebug() << "DB error:" << db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseHelper::disconnect()
{
    if (db.isOpen()) db.close();
}

bool DatabaseHelper::isConnected()
{
    return db.isOpen();
}

QSqlQuery DatabaseHelper::executeQuery(const QString& query)
{
    QSqlQuery q;
    q.exec(query);
    if (q.lastError().isValid())
        qDebug() << "Query error:" << q.lastError().text() << query;
    return q;
}

bool DatabaseHelper::executeNonQuery(const QString& query)
{
    QSqlQuery q;
    bool ok = q.exec(query);
    if (!ok)
        qDebug() << "Exec error:" << q.lastError().text() << query;
    return ok;
}

QSqlQueryModel* DatabaseHelper::getModel(const QString& query)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(query);
    if (model->lastError().isValid())
    {
        delete model;
        return nullptr;
    }
    return model;
}

bool DatabaseHelper::userExists(const QString& login)
{
    QSqlQuery q = executeQuery(QString("SELECT id FROM users WHERE login = '%1'").arg(login));
    return q.next();
}

bool DatabaseHelper::addUser(const QString& login, const QString& password, const QString& role)
{
    if (userExists(login)) return false;
    QString query = QString("INSERT INTO users (login, password_hash, role) VALUES ('%1', '%2', '%3')")
                        .arg(login).arg(password).arg(role);
    return executeNonQuery(query);
}