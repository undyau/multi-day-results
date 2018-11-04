#ifndef EVENTNAMES_H
#define EVENTNAMES_H

#include <QDialog>

namespace Ui {
class EventNames;
}

class EventNames : public QDialog
{
    Q_OBJECT

public:
    explicit EventNames(QWidget *parent = nullptr);
    ~EventNames();

private:
    Ui::EventNames *ui;
};

#endif // EVENTNAMES_H
