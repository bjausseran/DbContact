#include "databasemanager.h"

#include <QDir>
#include <QElapsedTimer>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    csv = new CsvManager();
    connect(csv, &CsvManager::fileRead, this, &DatabaseManager::onFileRead);
    connect(csv, &CsvManager::countDown, this, &DatabaseManager::countDown);
    connect(this, &DatabaseManager::requestCsvCreation, csv, &CsvManager::onCsvRequested);


    initQuery();
    //createTable();
}

void DatabaseManager::deleteData(QString fieldSearch, QString fieldValue)
{
    auto db = setupDatabase();
    QSqlQuery query;
    query.prepare("DELETE FROM contacts WHERE " + fieldSearch + " = ?");
    query.addBindValue(fieldValue);
    query.exec();
    cleanDatabase(db);
}

void DatabaseManager::updateData(QString fieldSearch, QString searchValue, QString fieldUpdated, QString newValue)
{

    auto db = setupDatabase();
    QSqlQuery query;
    query.prepare("UPDATE contacts SET " + fieldUpdated + "=:valueUpdate WHERE " + fieldSearch + "=:searchValue;");
    query.bindValue(":valueUpdate", newValue);
    query.bindValue(":searchValue", searchValue);
    query.exec();
    cleanDatabase(db);
}

void DatabaseManager::updateData(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues)
{

    auto db = setupDatabase();
    QString whereClause = "";
    for(int i = 0; i < fieldsSearched.length(); i++)
    {
        if(i != 0) whereClause.append(" AND ");
        whereClause.append(fieldsSearched[i] + "=:" + fieldsSearched[i]);
    }
    QString updateClause = "";
    for(int i = 0; i < fieldsUpdated.length(); i++)
    {
        if(i != 0) updateClause.append(", ");
        updateClause.append(fieldsUpdated[i] + "=:" + fieldsUpdated[i]);
    }

    QSqlQuery query;
    query.prepare("UPDATE contacts SET " + updateClause + " WHERE " + whereClause + ";");
    for(int i = 0; i < fieldsSearched.length(); i++)
    {
        query.bindValue(":"+fieldsSearched[i], fieldsValue[i]);
    }
    for(int i = 0; i < fieldsUpdated.length(); i++)
    {
        query.bindValue(":"+fieldsUpdated[i], newValues[i]);
    }
    query.exec();
    cleanDatabase(db);
}

void DatabaseManager::ReadFile(QString path)
{
    QDirIterator it(path, {"*.csv"}, QDir::Files);

    qDebug() << __FUNCTION__ << __LINE__ << path;

    while (it.hasNext()) {
        csv->readCsvfile(it.next());
    }
}

int DatabaseManager::selectDistinct(QString field)
{

    auto db = setupDatabase();
    QSqlQuery query;
    query.exec("SELECT DISTINCT "+ field +" FROM contacts;");

    QStringList list;
    while (query.next()) {
        list.append(query.value(0).toString());
    }


    if(query.first())
    {
      qDebug() << __FUNCTION__ << __LINE__ << "RESULT : " << query.value(0).toString();
    }
    else
      qDebug() << __FUNCTION__ << __LINE__ << "RESULT : " << query.first();

    return list.length();
    cleanDatabase(db);
}

void DatabaseManager::onFileRead(QStringList list)
{
    QList<Contact*> *contacts = new QList<Contact*>();

    for (int i = 0; i < list.length(); i++)
    {
        Contact *contact = new Contact();
        QStringList wordList = list[i].split(',');
        contact->GUID = wordList.at(1);
        contact->firstname = wordList.at(2);
        contact->lastname = wordList.at(3);
        contact->email = wordList.at(4);
        contact->tel = wordList.at(5);
        contact->category = wordList.at(6);
        contact->city = wordList.at(7);
        contact->birth_day = wordList.at(8);
        contact->country = wordList.at(9);
        contact->list = wordList.at(10);
        contact->company = wordList.at(11);


        contacts->append(contact);
    }


    addDataFromList(*contacts);
}

void DatabaseManager::onUpdateRequest(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues)
{

    auto db = setupDatabase();
    qDebug() << __FUNCTION__ << __LINE__ ;
    QString whereClause = "";
    for(int i = 0; i < fieldsSearched.length(); i++)
    {
        if(i != 0) whereClause.append(" AND ");
        whereClause.append(fieldsSearched[i] + "=:" + fieldsSearched[i]);
    }
    QString updateClause = "";
    for(int i = 0; i < fieldsUpdated.length(); i++)
    {
        if(i != 0) updateClause.append(", ");
        updateClause.append(fieldsUpdated[i] + "=:" + fieldsUpdated[i]);
    }

    QSqlQuery query;
    query.prepare("UPDATE contacts SET " + updateClause + " WHERE " + whereClause + ";");
    for(int i = 0; i < fieldsSearched.length(); i++)
    {
        query.bindValue(":"+fieldsSearched[i], fieldsValue[i]);
    }
    for(int i = 0; i < fieldsUpdated.length(); i++)
    {
        query.bindValue(":"+fieldsUpdated[i], newValues[i]);
    }
    query.exec();
    cleanDatabase(db);
}

void DatabaseManager::onExportCsvRequest(QStringList idList, QString path)
{

    qDebug() << __FUNCTION__ << __LINE__ ;
    selectDataForExport(idList, path);
}


void DatabaseManager::cleanDb(QSqlDatabase &db) {

    QSqlQuery query;
    query.exec("DROP TABLE contacts");
    if (query.lastError().isValid()) {
        qWarning() << "DROP TABLE" << query.lastError().text();
    }
    db.commit();
    db.close();
}

void DatabaseManager::addDataFromList(QList<Contact*> contacts)
{
    auto db = setupDatabase();
    db.transaction();

    QSqlQuery insertQuery(db);
    qDebug() << __FUNCTION__ << __LINE__ << contacts.length();
//    insertQuery.exec("pragma temp_store = memory");
//    insertQuery.exec("PRAGMA synchronous = normal");
//    insertQuery.exec("pragma mmap_size = 30000000000");
//    insertQuery.exec("PRAGMA journal_mode = wal");

    insertQuery.prepare("INSERT INTO contacts (GUID, firstname, lastname, email, tel, category, city, birth_day, country, list, company)"
                  "VALUES (:GUID, :firstname, :lastname, :email, :tel, :category, :city, :birth_day, :country, :list, :company)");

    //int lenght = sizeof(contacts)/sizeof (contacts[0]);

    auto count = 0;
    for (int i = 0; i < contacts.length(); i++) {

        insertQuery.bindValue(":GUID", contacts.at(i)->GUID);
        insertQuery.bindValue(":firstname", contacts.at(i)->firstname);
        insertQuery.bindValue(":lastname", contacts.at(i)->lastname);
        insertQuery.bindValue(":email", contacts.at(i)->email);
        insertQuery.bindValue(":tel", contacts.at(i)->tel);
        insertQuery.bindValue(":category", contacts.at(i)->category);
        insertQuery.bindValue(":city", contacts.at(i)->city);
        insertQuery.bindValue(":birth_day", contacts.at(i)->birth_day);
        insertQuery.bindValue(":country", contacts.at(i)->country);
        insertQuery.bindValue(":list", contacts.at(i)->list);
        insertQuery.bindValue(":company", contacts.at(i)->company);
        insertQuery.exec();

         emit countDown((contacts.length() + count * 100) / (contacts.length() * 2));
    }
    db.commit();

    cleanDatabase(db);
        for(int i = 0; i < contacts.count(); i++)
        {
            delete contacts.at(i);
        }
        //selectAll();
}

void DatabaseManager::requestData(QString fieldSearched, QString fieldValue)
{
    auto db = setupDatabase();
    QSqlQuery query;

    query.prepare("SELECT * FROM contacts WHERE " + fieldSearched + " = ?");
    query.addBindValue(fieldValue);

    if(!query.exec())
      qWarning() << "ERROR: " << query.lastError().text();

    QStringList list;

    if(query.first())
    {
      qDebug() << __FUNCTION__ << __LINE__ << "RESULT : " << query.value(0).toString();

        for(int i = 0; i < 12; i++)
        {
            qDebug() << __FUNCTION__ << __LINE__ << query.value(i);
            list.append(query.value(i).toString());
        }
    }
    else
      qDebug() << __FUNCTION__ << __LINE__ << "RESULT : " << query.first();


    emit sendContactData(list);
    cleanDatabase(db);
}


QFuture<void> DatabaseManager::selectDataForExport(QStringList idList, QString path)
{
    return QtConcurrent::run([this, idList, path]()
    {

        auto db = setupDatabase();
        QSqlQuery query;

        QString queryId = "(" + idList.at(0);
        for(int i = 1; i < idList.length(); i++)
        {
            queryId.append(", " + idList.value(i));
        }
        queryId.append(")");


        query.prepare("SELECT * FROM contacts WHERE id IN " + queryId);
        //query.bindValue(":ids", queryId);

        if(!query.exec())
          qWarning() << "ERROR: " << query.lastError().text();

        auto count = 0;
        QList<Contact*> *contacts = new QList<Contact*>();
        while (query.next()) {


                Contact *contact = new Contact();
                contact->id = query.value(0).toInt();
                contact->GUID = query.value(1).toString();
                contact->firstname = query.value(2).toString();
                contact->lastname = query.value(3).toString();
                contact->email = query.value(4).toString();
                contact->tel = query.value(5).toString();
                contact->category = query.value(6).toString();
                contact->city = query.value(7).toString();
                contact->birth_day = query.value(8).toString();
                contact->country = query.value(9).toString();
                contact->list = query.value(10).toString();
                contact->company = query.value(11).toString();

                contacts->append(contact);

                count++;
                emit countDown((count * 100) / (idList.count() * 2));
        }
        csv->writeCsvFile(*contacts, path);

        cleanDatabase(db);
    });
}

QFuture<QList<QStringList>> DatabaseManager::requestDataList(QString fieldSearched, QString fieldValue)
{
    return QtConcurrent::run([this, fieldSearched, fieldValue]()
    {

        auto db = setupDatabase();
        QSqlQuery query;

        int nbCategory = selectDistinct("category");
        int nbCompany = selectDistinct("company");
        int nbList = selectDistinct("list");

        query.exec("SELECT * FROM contacts WHERE " + fieldSearched + " = ?");
        query.addBindValue(fieldValue);

        if(!query.exec())
          qWarning() << "ERROR: " << query.lastError().text();
        QList<QStringList> list;
        qRegisterMetaType<QList<QStringList>>("QList<QStringList>");
        QList<QStringList> totalList;
        qRegisterMetaType<QList<QStringList>>("QList<QStringList>");

        while (query.next()) {

            QStringList item;
                for(int i = 0; i < 12; i++)
                {
                    //qDebug() << __FUNCTION__ << __LINE__ << query.value(i);
                    item.append(query.value(i).toString());

                }
                list.append(item);
                totalList.append(item);
                if(list.count() >= 10000)
                {
                    QFuture<void> future = QtConcurrent::run(this, &DatabaseManager::addResults, list);
                    list.clear();
                }


            }
        QFuture<void> future = QtConcurrent::run(this, &DatabaseManager::addResults, list);
        QFuture<void> futureStats = QtConcurrent::run(this, &DatabaseManager::addStats, totalList.count(), nbCategory, nbCompany, nbList);

        cleanDatabase(db);
        return totalList;

    });
}

QFuture<QList<QStringList>> DatabaseManager::selectAll()
{
    return QtConcurrent::run([this]()
    {

        auto db = setupDatabase();
        QSqlQuery query;

        int nbCategory = selectDistinct("category");
        int nbCompany = selectDistinct("company");
        int nbList = selectDistinct("list");

        query.exec("SELECT * FROM contacts");

        if(!query.exec())
          qWarning() << "ERROR: " << query.lastError().text();
        QList<QStringList> list;
        qRegisterMetaType<QList<QStringList>>("QList<QStringList>");
        QList<QStringList> totalList;
        qRegisterMetaType<QList<QStringList>>("QList<QStringList>");

        while (query.next()) {

            QStringList item;
                for(int i = 0; i < 12; i++)
                {
                    //qDebug() << __FUNCTION__ << __LINE__ << query.value(i);
                    item.append(query.value(i).toString());

                }
                list.append(item);
                totalList.append(item);
                if(list.count() >= 10000)
                {
                    QFuture<void> future = QtConcurrent::run(this, &DatabaseManager::addResults, list);
                    list.clear();
                }


            }
        QFuture<void> future = QtConcurrent::run(this, &DatabaseManager::addResults, list);
        QFuture<void> futureStats = QtConcurrent::run(this, &DatabaseManager::addStats, totalList.count(), nbCategory, nbCompany, nbList);

        cleanDatabase(db);
        return totalList;

    });

}
QSqlDatabase DatabaseManager::setupDatabase()
{

    const QString driver("QSQLITE");

    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir    dir(appDataLocation);
    if (!dir.exists()) {
        dir.mkdir(appDataLocation);
        qDebug() << __FUNCTION__ << __LINE__ << "mkdir" << appDataLocation;
    }
    QString dbPath = appDataLocation + "/contactdb.db";
    qDebug() << "dbPath" << dbPath;

    if (QSqlDatabase::isDriverAvailable(driver))
    {
        qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "-> Db path:" << dbPath;

        if (QSqlDatabase::contains())
        {
            if (!QSqlDatabase::database().open())
                qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "Error:" << QSqlDatabase::database().lastError().text();
            return QSqlDatabase::database();
        }
        else
        {
            QSqlDatabase db{ QSqlDatabase::addDatabase(driver) };
            db.setDatabaseName(dbPath);
            if (!db.open())
                qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "Error:" << db.lastError().text();
            return db;
        }
    }
    else
    {
        qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "No driver " << driver << " available";
    }
    return QSqlDatabase::database();
}

void DatabaseManager::initQuery()
{

    auto db = setupDatabase();

        QSqlQuery initQuery(db);
        initQuery.prepare("CREATE TABLE IF NOT EXISTS contacts ("
                                 "id        INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "GUID      STRING,"
                                 "firstname STRING,"
                                 "lastname  STRING,"
                                 "email     STRING,"
                                 "tel       STRING,"
                                 "category  STRING,"
                                 "city      STRING,"
                                 "birth_day STRING,"
                                 "country   STRING,"
                                 "list      STRING,"
                                 "company   STRING"
                                 ")");
        initQuery.exec();

        if (!initQuery.isActive())
            qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "-> Error:" << initQuery.lastError().text();

    cleanDatabase(db);
}

void DatabaseManager::cleanDatabase(QSqlDatabase &db)
{
    {
        qDebug() << "[ fn:" << __FUNCTION__ << "] -" << "( l:" << __LINE__ << ")" << "-> Conns:" << db.connectionNames();
        db.close();
    }
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}


