#ifndef CRUNNER_H
#define CRUNNER_H
#include <QString>
#include <map>

class CResult;

class CRunner
{
public:
    CRunner(QString a_Name, QString a_Club);
    ~CRunner();
    QString Name();
    QString Club();

    void AddResult(int a_Day, QString a_Position, QString a_TimeOrPoints);
    void AddResult(int a_Day, QString a_Position);
    void AddResult(int a_Day, int a_Position);
    CResult* ResultForDay(int a_Day);
    int Score();

private:
    std::map<int, CResult*> m_Results;
    QString m_Name;
    QString m_Club;
};

#endif // CRUNNER_H
