#include "cresult.h"

CResult::CResult(int a_Position, QString a_TimeOrPoints): m_Position(a_Position),
    m_TimeOrPoints(a_TimeOrPoints), m_NoResult(false)
{
}

CResult::CResult(int a_Position): m_Position(a_Position), m_TimeOrPoints("---"), m_NoResult(true)
{
}

int CResult::Position()
{
    return m_Position;
}

QString CResult::TimeOrPoints()
{
    return m_TimeOrPoints;
}

QString CResult::PositionForWeb()
{
    if (m_Position == 0)
        return QString("");

    if (this->NoResult())
        return QString("*%1").arg(Position());

    return QString("%1").arg(Position());
}
