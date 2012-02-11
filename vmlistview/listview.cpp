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


#include "listview.h"

#include <QMouseEvent>

#include <QDebug>
#include <QHeaderView>

ListView::ListView(QSqlTableModel* model, QWidget* parent): QTreeView(parent)
{
    this->model = model;
    setModel(model);

    setSortingEnabled(true);
    setItemsExpandable(false);
    setRootIsDecorated(false);

    header()->setMovable(true);

    QItemSelectionModel *selectionModel = new  QItemSelectionModel(model);
    setSelectionModel(selectionModel);

    connect(selectionModel, SIGNAL( selectionChanged(QItemSelection, QItemSelection) ),
            this, SLOT(newSelection(QItemSelection , QItemSelection )) );
}

void ListView::resizeAllColumnsToContent()
{
    for ( int i = 0; i < header()->count(); ++i )
        resizeColumnToContents(i);
}


void ListView::newSelection(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(deselected)
    Q_UNUSED(selected)

    qDebug() << "selection change";

//    QModelIndexList modelDataList = selected.indexes();

//    if ( !modelDataList.isEmpty() ) {
//        int index = this->model->fieldIndex("uuid");
//        qDebug() << modelDataList.at(index).data().toByteArray();
//        // emit currentMachine();
//    }
}


void ListView::mousePressEvent(QMouseEvent* event)
{
//     qDebug() << "mousePress" << event->pos();
    QTreeView::mousePressEvent(event);
}
