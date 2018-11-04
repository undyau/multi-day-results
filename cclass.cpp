#include "cclass.h"
#include <QString>
#include "crunner.h"
#include <vector>
#include "levenshtein.h"
#include <QDebug>
CClass::CClass(QString a_Name):m_Name(a_Name)
{
}

CClass::~CClass()
{
    for (auto i = m_Runners.begin(); i != m_Runners.end(); i++)
        delete i->second;
    m_Runners.clear();
}

QString CClass::Name()
{
    return m_Name;
}

CRunner* CClass::AddRunner(QString a_Name, QString a_Club)
{
 //   QString key = QString("%1%2").arg(a_Name).arg(a_Club);
    QString key(a_Name);
    key = key.toLower();
    if (m_Runners.find(key) == m_Runners.end())
        m_Runners[key] = new CRunner(a_Name, a_Club);

    return m_Runners[key];
}

std::vector<CRunner*>* CClass::RunnersByScore()
{
    std::vector<CRunner*>* ret = new std::vector<CRunner*>;
    for (auto i = m_Runners.begin(); i!= m_Runners.end(); i++)
        ret->push_back(i->second);

    struct {
        bool operator()(CRunner* a, CRunner* b)
        {
            return a->Score() < b->Score();
        }
    } customLess;
    std::sort(ret->begin(), ret->end(), customLess);
    return ret;
}


bool CClass::GotName(QString a_Name) const
{
    for (auto i = m_Runners.begin(); i!= m_Runners.end(); i++)
    {
        if (a_Name == i->second->Name())
            return true;
    }
    return false;
}

qreal CClass::FindNearName(QString& a_Name)
{
    //qDebug() << "Looking for name for " << a_Name;
    a_Name = a_Name.toLower();
    QString bestCandidate;
    qreal bestPct(0);

    LevenshteinCompare compare;

    for (auto i = m_Runners.begin(); i!= m_Runners.end(); i++)
    {
        QString candidate = i->second->Name();
        QString lastCandidate(candidate);
        candidate = candidate.toLower();

        if (compare.percentageDistance(a_Name, candidate) > bestPct)
        {
            bestPct = compare.percentageDistance(a_Name, candidate);
            bestCandidate = lastCandidate;
        }
    }

  //  qDebug() << "Best candidate for " << a_Name << "was" << bestCandidate << bestPct << "%";
    a_Name = bestCandidate;
    return bestPct;
}

