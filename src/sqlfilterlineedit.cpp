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


#include "sqlfilterlineedit.h"
#include "sqltools.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>

#include <QRegExp>

#include <QDebug>

SqlFilterLineEdit::SqlFilterLineEdit(QStringList *headerList, QWidget* parent): QLineEdit(parent)
{
    setPlaceholderText(tr("Filter with Wildcards [*%?_]"));

    this->headerList = headerList;

    timer = new QTimer();
    timer->setSingleShot(true);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(actionTimer(QString)));
    connect(timer, SIGNAL(timeout()), this, SLOT(makeQuickFilter()));
}

void SqlFilterLineEdit::actionTimer(QString inputText)
{
    timer->start(500);
}

void SqlFilterLineEdit::makeQuickFilter()
{
    QString inputText = text();

    if (inputText.isEmpty()) {
      emit newQuickFilter(QString());
      return;
    }

    inputText = replaceSplitCharakters(inputText);

    QList<QString> filterWords = inputText.split(" ");

    QString sql = createWhereStatement(filterWords);

    emit newQuickFilter(sql);
}

QString SqlFilterLineEdit::replaceSplitCharakters(QString string)
{
    string = string.replace(","," ");
    string = string.replace(";"," ");
    string = string.replace("."," ");
    string = string.trimmed();

    return string;
}

QString SqlFilterLineEdit::createWhereStatement(QList< QString > filterWords)
{
    QString sql; // jobid like "das" or jobid like "dies" or title like "das" or title like "dies" ...
    QString columnsHeaderName;
    QString sqlPart;

    foreach(QString filterWord, filterWords) {
        if ( hasWildcards(filterWord) )
            sqlPart = " like ";
        else
            sqlPart = " = ";

        sqlPart.append(SqlTools::convertToSqlConformString(filterWord));

        for( int i = 0; i < headerList->count(); ++i ){
            sql.append( headerList->at(i) );
            sql.append( sqlPart );
            sql.append(" or ");
        }
    }

    return SqlTools::removeThisRightOfString(sql, " or ");
}

bool SqlFilterLineEdit::hasWildcards(QString filter)
{
    QRegExp regexp;
    regexp.setPattern(QString(".*[%_\*\?]+.*"));

    if ( regexp.indexIn(filter) >= 0 )
        return true;

    return false;
}


SqlFilterLineEdit::~SqlFilterLineEdit()
{
    delete timer;
}

