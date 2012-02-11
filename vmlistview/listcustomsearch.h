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


#ifndef LISTCUSTOMSEARCH_H
#define LISTCUSTOMSEARCH_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

class SqlSearchRow;
class SqlSearchGroup;

class ListCustomSearch : public QWidget
{
    Q_OBJECT
public:
    explicit ListCustomSearch(QStringList* headerList, QWidget *parent);

Q_SIGNALS:
    void newCustomWhereStatement(QString);

public slots:
    void makeWhereStatement();

private:
    QPushButton *addRowButton;
    QPushButton *removeRowButton;

    QPushButton *testSearchButton;

    SqlSearchGroup *searchGroup;
};


class SqlSearchGroup: public QWidget
{
  Q_OBJECT
public:
    explicit SqlSearchGroup(QStringList* headerList, ListCustomSearch* parent = 0);

    QStringList getColumnNameList() const;
    QString getWhereStatement();

public slots:
    void addSearchRow();
    void removeSearchRow();

private:
    void rebuildLayout();

    QStringList *columnNameList;
    QList<SqlSearchRow*> searchRows;
};


class SqlSearchRow : public QWidget
{

public:
    SqlSearchRow(int count, SqlSearchGroup* parent = 0);
    QString getWherePart() const;

private:
    QComboBox *boolOperator;
    QComboBox *leftParanthesis;
    QComboBox *columnNameList;
    QComboBox *sqlOperator;
    QLineEdit *searchTherm;
    QComboBox *rightParanthesis;

    int id;
};

#endif // LISTCUSTOMSEARCH_H
