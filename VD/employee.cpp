#include "employee.h"

Employee::Employee() : lastName(""), firstName(""), idEmpl(0) {}

Employee::Employee(string t_lastName, string t_firstName,bool gender, QDate birthDate, unsigned int t_idEmpl) :
    lastName(t_lastName), firstName(t_firstName), gender(gender), birthDate(birthDate), idEmpl(t_idEmpl){}


Employee::Employee(string t_lastname, string t_firstName, bool gender, QDate birthDate) :
    lastName(t_lastname), firstName(t_firstName), gender(gender), birthDate(birthDate){}

Employee::~Employee()
{

}

void Employee::setFirstName(string first) { this->firstName = first;}
void Employee::setLastName(string last) { this->lastName = last;}
void Employee::setId(unsigned int id){ this->idEmpl = id;}
void Employee::setBirthDate(QDate t_birthDate){ this->birthDate = t_birthDate;}
void Employee::setGender(bool gender) { this->gender = gender;}


unsigned int Employee::getId() { return this->idEmpl;}
string Employee::getFirstName() const{ return this->firstName;}
string Employee::getLastName() const  { return this->lastName;}
QDate Employee::getBirthDate() const { return this->birthDate;}
bool Employee::getGender() const { return this->gender;}

