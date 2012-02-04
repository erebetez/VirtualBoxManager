#ifndef VMSTARTER_H
#define VMSTARTER_H

#include <QWidget>

class VmStarter: public QWidget
{
    Q_OBJECT

public:
    VmStarter(QWidget *parent);
    virtual ~VmStarter();

public slots:
    void startVirtualMachine();


private:


};

#endif // VMSTARTER_H
