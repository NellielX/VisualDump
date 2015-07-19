#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <stdlib.h>
#include <string>
#include <QDate>

using namespace std;
class Employee
{
public:
    Employee();
    Employee(string t_lastname, string t_firstName, bool gender, QDate birthDate);
    Employee(string t_lastName, string t_firstName, bool gender, QDate birthDate, unsigned int t_idEmpl);
    ~Employee();
    string getFirstName() const;
    string getLastName() const;
    void setFirstName(string last);
    void setLastName(string fisrt);
    void setId(unsigned int id);
    unsigned int getId();
    void setGender(bool gender);
    void setBirthDate(QDate t_birthDate);
    bool getGender() const;
    QDate getBirthDate() const;

 private:
    string lastName;
    string firstName;
    bool gender;
    QDate birthDate;
    unsigned int idEmpl;
};

#endif // EMPLOYEE_H
