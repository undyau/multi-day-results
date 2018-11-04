#ifndef CRESULT_H
#define CRESULT_H
#include <QString>

class CResult
{
public:
    CResult(int a_Position, QString a_TimeOrPoints);
    CResult(int a_Position);  // For DNF or missing result
    int Position();
    QString TimeOrPoints();
    bool NoResult() {return m_NoResult;}
    QString PositionForWeb();


private:
    int m_Position;
    QString m_TimeOrPoints;
    bool m_NoResult;
};

#endif // CRESULT_H
