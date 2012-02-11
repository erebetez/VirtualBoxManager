/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "sqltools.h"

#include <QDate>
#include <QtSql>
#include <QDebug>

SqlTools::SqlTools()
{
}

QString SqlTools::convertToSqlConformString(QVariant value)
{
    QString sql;
//     qDebug() << value;

    switch (value.type()) {

    case QVariant::String:
        sql.append(appendString(value.toString()));
        break;

    case QVariant::Date:
        sql.append(appendString(value.toDate().toString()));
        break;

    default:
        sql.append(removeToxicChars(value.toString()));
        break;
    }
    return sql;
}

QString SqlTools::appendString(QString value)
{
    value = removeToxicChars(value);

    if( isNumber(value) )
      return value;

    return QString("'" + value + "'");
}

bool SqlTools::isNumber(QString value)
{
    bool ok = false;
    value.toInt(&ok);
    return ok;
}

bool SqlTools::execSql(const QString sql)
{
    QSqlQuery query;

    if ( query.exec(sql) )
        return true;

    qDebug() << sql;
    qDebug() << query.lastError();
    return false;
}

QString SqlTools::removeThisRightOfString(QString string, QString toRemove)
{
    return string.left(string.length() - toRemove.length());
}


QString SqlTools::removeToxicChars(QString value)
{
    value = value.replace("?","_");
    value = value.replace("*","%");
    value = value.replace("'", "");
    return value;
}
