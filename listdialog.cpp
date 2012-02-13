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


#include "listdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QDebug>

ListDialog::ListDialog(QWidget* parent): QDockWidget(parent)
{
    setWindowTitle(tr("List"));

    model = new QSqlTableModel(this);

    model->setTable("virtualmachines");
    model->setEditStrategy( QSqlTableModel::OnManualSubmit);

    setupColumnHeaders();

    setupGui();

    update();
}

void ListDialog::setupColumnHeaders() {
    columnHeaders = new QStringList();

    for (int i = 0; i < model->columnCount(); ++i)
        columnHeaders->append(model->headerData(i, Qt::Horizontal).toString());
}

void ListDialog::setupGui() {
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);
    setWidget(widget);

    filterLine = new SqlFilterLineEdit(columnHeaders, this);
    layout->addWidget(filterLine, 0, 0);

    QPushButton *configButton = new QPushButton(tr("search"), this);
    configButton->setCheckable(true);
    layout->addWidget(configButton, 0, 2);

    customSearch = new ListCustomSearch(columnHeaders, this);
    layout->addWidget(customSearch, 1, 0, 1, 3);
    customSearch->hide();

    view = new ListView(this);
    view->setModel(model);
    view->setColumnHidden(0, true); // Don't show primary key uuid. Internal use only
    view->resizeAllColumnsToContent();

    QItemSelectionModel *selectionModel = new  QItemSelectionModel(model);

    view->setSelectionModel(selectionModel);

    connect(selectionModel, SIGNAL( selectionChanged(QItemSelection, QItemSelection) ),
            this, SLOT(newSelection(QItemSelection , QItemSelection )) );


    layout->addWidget(view, 2, 0, 1, 3);

    connect(configButton, SIGNAL(pressed()), this, SLOT(showHideCustomSearchWidget()));

    connect(filterLine, SIGNAL(newQuickFilter(QString)), this, SLOT(setQuickFilter(QString)));
    connect(customSearch, SIGNAL(newCustomWhereStatement(QString)), this, SLOT(setCustomFilter(QString)));
}

void ListDialog::newSelection(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(deselected)

    qDebug() << "selection change";

    QModelIndexList modelDataList = selected.indexes();

    if ( !modelDataList.isEmpty() ) {
        int index = this->model->fieldIndex("uuid");

        m_currentMachine = modelDataList.at(index).data().toByteArray();

        qDebug() << m_currentMachine;
        emit currentMachine(m_currentMachine);
    }
}

QByteArray ListDialog::currentMachine() const {
    return m_currentMachine;
}

void ListDialog::setQuickFilter(QString sql)
{
    currentQuickFilter = sql;
    setModelFilter();
}

void ListDialog::setCustomFilter(QString sql)
{
    currentCustomFilter = sql;
    setModelFilter();
}

void ListDialog::setModelFilter()
{
    qDebug() << "custom" << currentCustomFilter;
    qDebug() << "quick" << currentQuickFilter;

    QString merger = QString();

    if (!currentCustomFilter.isEmpty() && !currentQuickFilter.isEmpty()) {
        merger = QString(" and (");
        currentQuickFilter.append(")");
    }

    model->setFilter(currentCustomFilter + merger + currentQuickFilter);
}

void ListDialog::showHideCustomSearchWidget()
{
    if ( customSearch->isVisible() ) {
        customSearch->hide();
    } else {
        customSearch->show();
    }
}

void ListDialog::update()
{
    model->select();
}

ListDialog::~ListDialog()
{
    delete model;
    delete view;
    delete filterLine;
    delete customSearch;
    delete columnHeaders;
}

