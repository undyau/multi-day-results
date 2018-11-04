/* Levenshtein
 * Copyright (C) 2010  Andre L. V. Loureiro
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QVector>

#include "levenshtein.h"

bool percentageSort(QPair<qreal, QString> a, QPair<qreal, QString> b) {
    return a.first > b.first;
}

LevenshteinCompare::LevenshteinCompare(QObject *parent) : QObject(parent)
{
}

LevenshteinCompare::~LevenshteinCompare()
{
}

int LevenshteinCompare::editDistance(const QString &str1, const QString &str2)
{
    int len1, len2;
    len1 = str1.size();
    len2 = str2.size();

    QVector<QVector<int> > d(len1+1, QVector<int>(len2+1));

    d[0][0] = 0;

    for (int i=1; i <= len1; ++i) {
        d[i][0] = i;
    }
    for (int j=1; j <= len2; ++j) {
        d[0][j] = j;
    }

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            d[i][j] = qMin(qMin(d[i-1][j] + 1, d[i][j-1] + 1), d[i-1][j-1] + (str1.data()[i-1] == str2.data()[j-1] ? 0 : 1));
        }
    }

    return d[len1][len2];
}

qreal LevenshteinCompare::percentageDistance(const QString &str1, const QString &str2, Sort sort)
{
    int distance = editDistance(str1, str2);
    qreal percentage = (1.0 - qreal(distance)/qreal(str2.size())) * 100;
    QPair<qreal, QString> item;
    item.first = percentage;
    item.second = str2;

    m_values.append(item);
    if (sort == LevenshteinCompare::Sorted) {
       qSort(m_values.begin(), m_values.end(), percentageSort);
    }

    return percentage;
}

QList<QPair<qreal, QString> > LevenshteinCompare::values() const
{
    return m_values;
}

QString LevenshteinCompare::moreSimilar() const
{
    QString str = "";
    if (!m_values.isEmpty()) {
        QPair<qreal, QString> item = m_values.first();
        str = item.second;
    }
    return str;
}
