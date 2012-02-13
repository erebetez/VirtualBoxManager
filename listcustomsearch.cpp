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


#include "listcustomsearch.h"
#include "sqltools.h"

#include <QGridLayout>

#include <QDebug>

ListCustomSearch::ListCustomSearch(QStringList* headerList, QWidget* parent): QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->setMargin(0);

    searchGroup = new SqlSearchGroup(headerList, this);
    layout->addWidget(searchGroup, 0, 0, 4, 2);

    QPushButton *saveButton = new QPushButton(tr("save"), this);
    layout->addWidget(saveButton, 0, 3);

    addRowButton = new QPushButton(tr("add"), this);
    layout->addWidget(addRowButton, 1, 3);

    removeRowButton = new QPushButton(tr("remove"), this);
    layout->addWidget(removeRowButton, 2, 3);

    testSearchButton = new QPushButton(tr("test"), this);
    layout->addWidget(testSearchButton, 3, 3);


    connect(addRowButton, SIGNAL(pressed()), searchGroup, SLOT(addSearchRow()));
    connect(removeRowButton, SIGNAL(pressed()), searchGroup, SLOT(removeSearchRow()));
    connect(testSearchButton, SIGNAL(pressed()), this, SLOT(makeWhereStatement()));
}

void ListCustomSearch::makeWhereStatement()
{
     QString sql = searchGroup->getWhereStatement();
     emit newCustomWhereStatement(sql);
}


SqlSearchGroup::SqlSearchGroup(QStringList* headerList, ListCustomSearch* parent): QWidget(parent)
{
    this->columnNameList = headerList;

    addSearchRow();
}

void SqlSearchGroup::addSearchRow()
{
    searchRows.append(new SqlSearchRow(searchRows.count(), this));
    rebuildLayout();
}

void SqlSearchGroup::removeSearchRow()
{
    if ( searchRows.count() > 1 ) {
        delete searchRows.last();
        searchRows.removeLast();
        rebuildLayout();
    }
}

void SqlSearchGroup::rebuildLayout()
{
    if ( layout() ) {
        delete layout();
    }

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);

    foreach( QWidget *row, searchRows ){
        verticalLayout->addWidget(row);
    }

    verticalLayout->setMargin(0);
    verticalLayout->setSpacing(0);
}

QString SqlSearchGroup::getWhereStatement()
{
    QString sqlWhere;
    for( int i = 0; i < searchRows.count(); ++i ){
        sqlWhere.append(searchRows.at(i)->getWherePart());
    }

    return sqlWhere;
}


QStringList SqlSearchGroup::getColumnNameList() const
{
    QStringList columnList;

    for( int i = 0; i < columnNameList->count(); ++i )
        columnList.append(columnNameList->at(i));

    return columnList;
}



SqlSearchRow::SqlSearchRow(int count, SqlSearchGroup* parent): QWidget(parent)
{
    id = count;

    qDebug() << id;

    QHBoxLayout *layout = new QHBoxLayout(this);

    boolOperator = new QComboBox(this);
    boolOperator->addItem("and");
    boolOperator->addItem("or");
    layout->addWidget(boolOperator);

    if (id == 0) {
        boolOperator->setHidden(true);
    }

    leftParanthesis = new QComboBox(this);
    leftParanthesis->addItem("");
    leftParanthesis->addItem("(");
    leftParanthesis->addItem(")");
    layout->addWidget(leftParanthesis);

    columnNameList = new QComboBox(this);
    columnNameList->addItems(parent->getColumnNameList());
    layout->addWidget(columnNameList);

    sqlOperator = new QComboBox(this);
    sqlOperator->addItem("like");
    sqlOperator->addItem("not like");
    sqlOperator->addItem("<");
    sqlOperator->addItem(">");
    sqlOperator->addItem("<=");
    sqlOperator->addItem(">=");
    sqlOperator->addItem("=");
    sqlOperator->addItem("<>");
    layout->addWidget(sqlOperator);

    searchTherm = new QLineEdit(this);
    searchTherm->setPlaceholderText(tr("search term"));
    layout->addWidget(searchTherm);

    rightParanthesis = new QComboBox(this);
    rightParanthesis->addItem("");
    rightParanthesis->addItem("(");
    rightParanthesis->addItem(")");
    layout->addWidget(rightParanthesis);

}

QString SqlSearchRow::getWherePart() const
{
  QString sqlPart;

  if (searchTherm->text().isEmpty()) {
     return sqlPart;
  }

  if (id != 0)  {
     sqlPart.append(boolOperator->currentText());
  }

  sqlPart.append(" ");
  sqlPart.append(leftParanthesis->currentText());
  sqlPart.append(" ");
  sqlPart.append(columnNameList->currentText());
  sqlPart.append(" ");
  sqlPart.append(sqlOperator->currentText());
  sqlPart.append(" ");
  sqlPart.append( SqlTools::convertToSqlConformString(searchTherm->text()) );
  sqlPart.append(" ");
  sqlPart.append(rightParanthesis->currentText());
  sqlPart.append(" ");

//   qDebug() << "rowPart:" << sqlPart;
  return sqlPart;
}
