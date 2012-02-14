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


#ifndef SQLFILTERLINEEDIT_H
#define SQLFILTERLINEEDIT_H

#include <QLineEdit>
#include <QTimer>


class SqlFilterLineEdit : public QLineEdit
{
Q_OBJECT
public:
    SqlFilterLineEdit(QStringList* headerList, QWidget* parent = 0);
    virtual ~SqlFilterLineEdit();

Q_SIGNALS:
   void newQuickFilter(QString);

private slots:
    void makeQuickFilter();
    void actionTimer(QString);

private:
  QString replaceSplitCharakters(QString string);
  QString createWhereStatement(QList<QString> filterWords);
  bool hasWildcards(QString filter);

  QStringList *headerList;
  QTimer *timer;
};

#endif // SQLFILTERLINEEDIT_H
