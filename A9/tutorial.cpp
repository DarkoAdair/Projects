#include "tutorial.h"
#include "ui_tutorial.h"

Tutorial::Tutorial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: rgba(0,0,0,0)");
    ui->tabWidget->setTabText(0, "UI");
    ui->tabWidget->setTabText(1, "Rules");




}

Tutorial::~Tutorial()
{
    delete ui;
}
