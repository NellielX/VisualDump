#include "maintabwidget.h"
#include "ui_maintabwidget.h"

MainTabWidget::MainTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::MainTabWidget)
{
    ui->setupUi(this);

}

MainTabWidget::~MainTabWidget()
{
    delete ui;
}
