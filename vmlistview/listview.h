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


#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QTreeView>
#include <QItemSelectionModel>
#include <QSqlTableModel>

class ListView : public QTreeView
{
    Q_OBJECT
public:
    explicit ListView(QSqlTableModel* model, QWidget* parent = 0);

    void resizeAllColumnsToContent();

Q_SIGNALS:
    void currentMachine(QByteArray);

private slots:
    void newSelection(QItemSelection selected, QItemSelection deselected);

private:
    QSqlTableModel *model;

protected:
    virtual void mousePressEvent( QMouseEvent * event );
};

#endif // LISTVIEW_H