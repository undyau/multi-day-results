#ifndef EVENTNAMES_H
#define EVENTNAMES_H

#include <QDialog>
#include <QSettings>
#include <QLineEdit>

namespace Ui {
class EventNames;
}

class EventNames : public QDialog
{
    Q_OBJECT

public:
    explicit EventNames(QWidget *parent = nullptr);
    ~EventNames();

    QString OverallName() const;
    QString Event1Name() const;
    QString Event2Name() const;
    QString Event3Name() const;
    QString Event4Name() const;
    QString Event5Name() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EventNames *ui;
    void SetName(QSettings &a_Settings, QString a_NameKey, QLineEdit *a_Control);
};

#endif // EVENTNAMES_H
