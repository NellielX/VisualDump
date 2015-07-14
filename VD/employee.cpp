#include "employee.h"

Employee::Employee() : lastName(""), firstName(""), idEmpl(0) {}

Employee::Employee(string t_lastName, string t_firstName, unsigned int t_idEmpl) : lastName(t_lastName), firstName(t_firstName), idEmpl(t_idEmpl){}

Employee::~Employee()
{

}


string Employee::getFirstName() const{ return this->firstName;}
string Employee::getLastName() const  { return this->lastName;}

