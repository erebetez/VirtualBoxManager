#include "vmstarter.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QHash>
#include <QtSql/QSqlQuery>

#include "virtualboximpl.h"

#include <QDebug>


VmStarter::VmStarter(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);


    QList<QByteArray> virtualMachineList = VirtualBoxImpl::instance()->listVmUUIDs();

    qDebug() << virtualMachineList;


    QTreeWidget *treeWidget = new QTreeWidget();
    QList<QTreeWidgetItem *> items;

    foreach(QByteArray vitualMachine, virtualMachineList){
        QHash<QByteArray, QByteArray> vitualMachineInfo = VirtualBoxImpl::instance()->listVmInfo(vitualMachine);

        qDebug() << vitualMachineInfo["name"];

        QTreeWidgetItem *virtualMachines = new QTreeWidgetItem(treeWidget);
        virtualMachines->setText(0, vitualMachineInfo.value("name"));

        QTreeWidgetItem *virtualMachineInfo = new QTreeWidgetItem(virtualMachines);
        virtualMachineInfo->setText(0, tr("UUID:") + vitualMachineInfo.value("UUID"));

        QTreeWidgetItem *virtualMachineInfo2 = new QTreeWidgetItem(virtualMachines);
        virtualMachineInfo2->setText(0, tr("Type:") + vitualMachineInfo.value("ostype"));

        items.append(virtualMachines);
    }


    treeWidget->insertTopLevelItems(0, items);


    // TODO Put the data into a db. Use SQLTableModel to display.
//    QSqlQuery query;
//    query.prepare("INSERT INTO vm (uuid, name, state, memory) "
//                  "VALUES (?, ?, ?, ?)");

//    query.bindValue(0, 1001);
//    query.bindValue(1, "Bart");
//    query.bindValue(2, "Simpson");
//    query.exec();




    QPushButton *vmStartButton = new QPushButton(tr("Start"), this);


    layout->addWidget(treeWidget);
    layout->addWidget(vmStartButton);

    connect( vmStartButton, SIGNAL( pressed() ),
             this, SLOT( startVirtualMachine() ) );

}


VmStarter::~VmStarter(){
}


void VmStarter::startVirtualMachine(){
    qDebug() << "Button Pressend";

}
