#ifndef CCLASS_H
#define CCLASS_H
#include <QString>
#include <map>
#include <vector>


class CRunner;
class MainWindow;
class CClass
{
public:
    CClass(QString a_Name);
    ~CClass();
    QString Name();
    CRunner* AddRunner(QString a_Name, QString a_Club);
    std::vector<CRunner*>* RunnersByScore();

    std::map<QString, CRunner*> m_Runners;

    bool GotName(QString a_Name) const;
    qreal FindNearName(QString& name);


private:
    QString m_Name;
};

#endif // CCLASS_H
