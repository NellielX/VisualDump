#include "mainwindow.h"
#include "connectiondialog.h"
#include <QApplication>
#include "sqlconnection.h"
#include <QDial>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectionDialog w;

    w.setFixedSize(w.size());
    qDebug() << SqlConnection::createConnection();
    if (w.exec() == QDialog::Accepted)
    {
        MainWindow mainWindow;
        mainWindow.show();
        return a.exec();
    }
    return 0;
}
