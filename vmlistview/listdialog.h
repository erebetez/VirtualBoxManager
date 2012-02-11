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


#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include "listview.h"
#include "sqlfilterlineedit.h"
#include "listcustomsearch.h"

#include <QtGui/QDockWidget>
#include <QSqlTableModel>

class ListDialog : public QDockWidget
{
    Q_OBJECT

public:
    explicit ListDialog(QWidget* parent = 0);
    virtual ~ListDialog();

Q_SIGNALS:
    void currentJob(int);

public slots:
    void setQuickFilter(QString);
    void setCustomFilter(QString);
    void update();

private slots:
    void showHideCustomSearchWidget();

private:
    void setupGui();
    void setupColumnHeaders();
    void setModelFilter();

    QString currentCustomFilter;
    QString currentQuickFilter;

    QSqlTableModel *model;
    QStringList *columnHeaders;

    SqlFilterLineEdit *filterLine;
    ListCustomSearch *customSearch;
    ListView *view;

    QPushButton *customSearchButton;
};

#endif // LISTDIALOG_H
