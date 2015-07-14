#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H
#include <QPushButton>
#include <QDialog>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConnectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
