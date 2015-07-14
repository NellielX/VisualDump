#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include "sqlconnection.h"
#include <QPushButton>
ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.length() > 0 && ui->lineEdit_2->text().length() > 0){
        if(!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()){
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }
}


void ConnectionDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.length() > 0 && ui->lineEdit->text().length() > 0){
        if(!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()){
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    }

}

void ConnectionDialog::on_buttonBox_accepted()
{

    if(SqlConnection::isUserAcessGranted(ui->lineEdit->text(), ui->lineEdit_2->text()))
    {
        this->accept();
    }
    else{
        qDebug() << "Err non conencte";
    }
}

void ConnectionDialog::on_buttonBox_rejected()
{
    this->reject();
}
