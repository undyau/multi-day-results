#include "crunner.h"
#include "cresult.h"
#include <QMessageBox>
#include <QDebug>

CRunner::CRunner(QString a_Name, QString a_Club) : m_Name(a_Name), m_Club(a_Club)
{
}

CRunner::~CRunner()
{
    for (auto i = m_Results.begin(); i != m_Results.end(); i++)
        delete i->second;
    m_Results.clear();
}

QString CRunner::Name()
{
    return m_Name;
}

QString CRunner::Club()
{
    return m_Club;
}

void CRunner::AddResult(int a_Day, QString a_Position, QString a_TimeOrPoints)
{
    bool ok;
    int pos = a_Position.toInt(&ok);
    if (ok && pos >= 1)
        m_Results[a_Day] = new CResult(pos, a_TimeOrPoints);
}

void CRunner::AddResult(int a_Day, QString a_Position)
{
    bool ok;
    int pos = a_Position.toInt(&ok);
    if (ok && pos >= 1)
         m_Results[a_Day] = new CResult(pos);
    else
        QMessageBox::information(0, "Illogical Captain" , "Logic problem in CRunner::AddResult(int a_Day, QString a_Position)");
}

void CRunner::AddResult(int a_Day, int a_Position)
{
    if (a_Position > 0)
        m_Results[a_Day] = new CResult(a_Position);
}

CResult *CRunner::ResultForDay(int a_Day)
{
    if (m_Results.find(a_Day) != m_Results.end())
        return m_Results[a_Day];
    else
        return nullptr;
}

int CRunner::Score()
{
    std::vector<int> scores;
    int score(0);
    for (auto i = m_Results.begin(); i!= m_Results.end(); i++)
        scores.push_back(i->second->Position());

    std::sort(scores.begin(), scores.end());

    for (unsigned int i = 0; i < 4 && i < scores.size(); i++)
        score += scores.at(i);
    return score;
}
