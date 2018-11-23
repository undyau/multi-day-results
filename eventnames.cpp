#include "eventnames.h"
#include "ui_eventnames.h"
#include "QSettings"

EventNames::EventNames(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventNames)
{
    ui->setupUi(this);
    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "MultiDayResults");
    settings.beginGroup("Names");
    SetName(settings, "Overall", ui->lineEditOverall, "Xmas 5 Days 2018");
    SetName(settings, "Day1", ui->lineEditEvent1, "Milsons Point");
    SetName(settings, "Day2", ui->lineEditEvent2, "Centennial Park");
    SetName(settings, "Day3", ui->lineEditEvent3, "Beacon Hill");
    SetName(settings, "Day4", ui->lineEditEvent4, "Manly");
    SetName(settings, "Day5", ui->lineEditEvent5, "Sydney Uni");
    settings.endGroup();
}

EventNames::~EventNames()
{
    delete ui;
}

void EventNames::SetName(QSettings& a_Settings, QString a_NameKey, QLineEdit* a_Control, QString a_Default)
{
    QString name = a_Settings.value(a_NameKey).toString();
    if (!name.isEmpty())
        a_Control->setText(name);
    else
        a_Control->setText(a_Default);
}

void EventNames::on_buttonBox_accepted()
{
    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "MultiDayResults");
    settings.beginGroup("Names");
    settings.setValue("Overall", ui->lineEditOverall->text());
    settings.setValue("Day1", ui->lineEditEvent1->text());
    settings.setValue("Day2", ui->lineEditEvent2->text());
    settings.setValue("Day3", ui->lineEditEvent3->text());
    settings.setValue("Day4", ui->lineEditEvent4->text());
    settings.setValue("Day5", ui->lineEditEvent5->text());
    settings.endGroup();
}

QString EventNames::Event5Name() const
{
    return ui->lineEditEvent5->text();
}

QString EventNames::Event4Name() const
{
    return ui->lineEditEvent4->text();
}

QString EventNames::Event3Name() const
{
    return ui->lineEditEvent3->text();
}

QString EventNames::Event2Name() const
{
    return ui->lineEditEvent2->text();
}

QString EventNames::Event1Name() const
{
    return ui->lineEditEvent1->text();
}

QString EventNames::OverallName() const
{
    return ui->lineEditOverall->text();
}

