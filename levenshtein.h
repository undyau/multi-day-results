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

#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QString>

class LevenshteinCompare : public QObject
{
    Q_OBJECT
    Q_ENUMS(Sort)

public:
    LevenshteinCompare(QObject *parent = 0);
    ~LevenshteinCompare();

    enum Sort {Sorted, Unsorted};

    int editDistance(const QString &str1, const QString &str2);
    qreal percentageDistance(const QString &str1, const QString &str2, Sort sort=Sorted);
    QList<QPair<qreal, QString> > values() const;

    QString moreSimilar() const;

private:
    QList<QPair<qreal, QString> > m_values;

};

#endif // LEVENSHTEIN_H
