#include "eventnames.h"
#include "ui_eventnames.h"

EventNames::EventNames(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventNames)
{
    ui->setupUi(this);
}

EventNames::~EventNames()
{
    delete ui;
}
