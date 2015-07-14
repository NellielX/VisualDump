#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <stdlib.h>
#include <string>

using namespace std;
class Employee
{
public:
    Employee();
    Employee(string t_lastName, string t_firstName, unsigned int t_idEmpl);
    ~Employee();
    string getFirstName() const;
    string getLastName() const;
    void setFirstName(string last);
    void setLastName(string fisrt);

 private:
    string lastName;
    string firstName;
    unsigned int idEmpl;
};

#endif // EMPLOYEE_H
