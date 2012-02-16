#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <QByteArray>

class Hypervisor
{
public:
    Hypervisor();

    QByteArray name() const { return m_name;}
    QByteArray adress() const { return m_adress;}
    QByteArray typ() const { return m_typ;}
    QByteArray user() const { return m_user;}

    void setName(const QByteArray name) {m_name = name;}
    void setAdress(const QByteArray adress) {m_adress = adress;}
    void setTyp(const QByteArray typ) {m_typ = typ;}
    void setUser(const QByteArray user ) {m_user = user;}

private:
    QByteArray m_name;
    QByteArray m_adress;
    QByteArray m_typ;
    QByteArray m_user;
};

#endif // HYPERVISOR_H
