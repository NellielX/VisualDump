#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>

namespace Ui {
class MainTabWidget;
}

class MainTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MainTabWidget(QWidget *parent = 0);
    ~MainTabWidget();

private:
    Ui::MainTabWidget *ui;
};

#endif // MAINTABWIDGET_H
