#include "vmstarter.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTreeWidget>

#include "virtualboximpl.h"

#include <QDebug>


VmStarter::VmStarter(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);


    QStringList virtualMachineList = VirtualBoxImpl::instance()->listVm();


    QTreeWidget *treeWidget = new QTreeWidget();

    treeWidget->setColumnCount(2);

    QList<QTreeWidgetItem *> items;

    foreach(QString vm, virtualMachineList ){
        QTreeWidgetItem *virtualMachines = new QTreeWidgetItem(treeWidget);
        virtualMachines->setText(0, vm.split(' ').at(0));

        QTreeWidgetItem *virtualMachineInfo = new QTreeWidgetItem(virtualMachines);
        virtualMachineInfo->setText(0, tr("GoGo"));
        virtualMachineInfo->setText(1, vm.split(' ').at(1));


        items.append(virtualMachines);
    }

    treeWidget->insertTopLevelItems(0, items);



//    vmListWidget->addItems(virtualMachineList);


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
