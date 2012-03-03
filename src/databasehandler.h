#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>

class DatabaseHandler
{
public:
    static bool connectToDatabase(const QString &dataBaseFileName);


private:
    static void initDatabase();
    DatabaseHandler();

};

#endif // DATABASEHANDLER_H
