#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H
#include <QtCore>
#include <employee.h>

class SqlConnection
{
private:


public:
    static bool isUserAcessGranted(const QString &username, const QString &password);
    static bool createConnection();
    static void insertEmployee(Employee &emp);
    static vector<int> getEmployeeIdList();
};

#endif // SQLCONNECTION_H
