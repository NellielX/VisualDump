#include "sqlconnection.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

bool SqlConnection::isUserAcessGranted(const QString &username, const QString &password){
    QSqlQuery query;
    query.prepare("SELECT EXISTS(SELECT username FROM vsapiuser WHERE username=? AND password=?)");
    query.addBindValue(username);
    query.addBindValue(password);
    if(!query.exec()){
        qDebug() << query.lastError();
        return false;
    }
    while(query.next())
    {
        return query.value(0).toBool();
    }
    return false;
}


bool SqlConnection::createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("visualdump");
    db.setUserName("root");
//    db.setPassword("");
    return db.open();
    }

void SqlConnection::insertEmployee(Employee &emp){

    QSqlQuery query;
    query.prepare("INSERT INTO employee (firstName, lastName, gender, birthDate) VALUES(?,?,?,?)");
    query.addBindValue(QVariant(QString(emp.getFirstName().c_str())));
    query.addBindValue(QVariant(QString(emp.getLastName().c_str())));
    query.addBindValue(emp.getGender());
    query.addBindValue(emp.getBirthDate()); /* Insert into table employee then set id to employee with lastinsertID*/
    if (query.exec()) emp.setId(query.lastInsertId().toInt());
}



