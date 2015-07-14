#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H
#include <QtCore>

class SqlConnection
{
private:


public:
    static bool isUserAcessGranted(const QString &username, const QString &password);
    static bool createConnection();
};

#endif // SQLCONNECTION_H
