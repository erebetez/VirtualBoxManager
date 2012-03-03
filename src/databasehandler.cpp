#include "databasehandler.h"

#include <QtSql>

DatabaseHandler::DatabaseHandler()
{

}


bool DatabaseHandler::connectToDatabase(const QString &dataBaseFileName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dataBaseFileName);

    if (!db.open()) {
        qDebug() << db.lastError();
        return false;
    }

    initDatabase();

    return true;
}


void DatabaseHandler::initDatabase() {
    QSqlQuery query;
    query.exec("create table IF NOT EXISTS virtualmachines (vmuuid PRIMARY KEY, "
               "name, "
               "hypervisor, "
               "ostype, "
               "state, "
               "memorymax, "
               "cpumax, "
               "available"
               ")"
              );

    query.exec("create table IF NOT EXISTS harddiscs (hduuid PRIMARY KEY, "
               "vmuuid, "
               "location, "
               "maxsize, "
               "currentsize, "
               "available"
               ")"
              );

    query.exec("create table IF NOT EXISTS hdused (vmuuid, "
               "hduuid "
               ")"
              );

    query.exec("create table IF NOT EXISTS hypervisors (hypervisor PRIMARY KEY, "
               "host, "
               "typ, "
               "user  "
               ")"
              );

//    qDebug() << query.lastError();
}
