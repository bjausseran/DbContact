#include "databasemanager.h"

#include <QDir>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    csv = new CsvManager();
    connect(csv, &CsvManager::fileRead, this, &DatabaseManager::onFileRead);
    connect(this, &DatabaseManager::requestCsvCreation, csv, &CsvManager::onCsvRequested);

    createTable();
}

void DatabaseManager::deleteData(QString fieldSearch, QString fieldValue)
{
    QSqlQuery query;
    query.prepare("DELETE FROM contacts WHERE " + fieldSearch + " = ?");
    query.addBindValue(fieldValue);
    query.exec();
}

void DatabaseManager::updateData(QString fieldSearch, QString searchValue, QString fieldUpdated, QString newValue)
{
    QSqlQuery query;
    query.prepare("UPDATE contacts SET " + fieldUpdated + "=:valueUpdate WHERE " + fieldSearch + "=:searchValue;");
    query.bindValue(":valueUpdate", newValue);
    query.bindValue(":searchValue", searchValue);
    query.exec();
}

void DatabaseManager::updateData(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues)
{
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

}

void DatabaseManager::ReadFile(QString path)
{
    csv->readCsvfile(path);
}

int DatabaseManager::selectDistinct(QString field)
{
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
}

void DatabaseManager::onFileRead(QStringList list)
{
    //const int listLenght = list.length();
    //Contact contacts[listLenght] = {};
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

    addData(*contacts);
    for(int i = 0; i < contacts->length(); i++)
    {
        delete contacts->at(i);
    }
}

void DatabaseManager::onUpdateRequest(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues)
{

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
}


void DatabaseManager::cleanDb() {

    QSqlQuery query;
    query.exec("DROP TABLE contacts");
    if (query.lastError().isValid()) {
        qWarning() << "DROP TABLE" << query.lastError().text();
    }
    db.commit();
    db.close();
}


void DatabaseManager::addData(Contact contacts[])
{

    QSqlQuery query;
    query.exec("pragma temp_store = memory");
    query.exec("PRAGMA synchronous = normal");
    query.exec("pragma mmap_size = 30000000000");
    query.exec("PRAGMA journal_mode = wal");

    db.transaction();
    query.prepare("INSERT INTO contacts (id, GUID, firstname, lastname, tel, category, city, birth_day, country, list, company)"
                  "VALUES (:GUID, :firstname, :lastname, :tel, :category, :city, :birth_day, :country, :lis, :company)");

    int lenght = sizeof(contacts)/sizeof (contacts[0]);

    for (int i = 0; i < lenght; i++) {

        query.bindValue(":GUID", contacts[i].GUID);
        query.bindValue(":firstname", contacts[i].firstname);
        query.bindValue(":lastname", contacts[i].lastname);
        query.bindValue(":tel", contacts[i].tel);
        query.bindValue(":category", contacts[i].category);
        query.bindValue(":city", contacts[i].city);
        query.bindValue(":birth_day", contacts[i].birth_day);
        query.bindValue(":country", contacts[i].country);
        query.bindValue(":list", contacts[i].list);
        query.bindValue(":company", contacts[i].company);
        query.exec();
    }
    db.commit();
}
void DatabaseManager::addData(QList<Contact*> contacts)
{

    qDebug() << __FUNCTION__ << __LINE__ << contacts.length();
    QSqlQuery query;
    query.exec("pragma temp_store = memory");
    query.exec("PRAGMA synchronous = normal");
    query.exec("pragma mmap_size = 30000000000");
    query.exec("PRAGMA journal_mode = wal");

    db.transaction();
    query.prepare("INSERT INTO contacts (GUID, firstname, lastname, email, tel, category, city, birth_day, country, list, company)"
                  "VALUES (:GUID, :firstname, :lastname, :email, :tel, :category, :city, :birth_day, :country, :list, :company)");

    //int lenght = sizeof(contacts)/sizeof (contacts[0]);

    for (int i = 0; i < contacts.length(); i++) {

        query.bindValue(":GUID", contacts.at(i)->GUID);
        query.bindValue(":firstname", contacts.at(i)->firstname);
        query.bindValue(":lastname", contacts.at(i)->lastname);
        query.bindValue(":email", contacts.at(i)->email);
        query.bindValue(":tel", contacts.at(i)->tel);
        query.bindValue(":category", contacts.at(i)->category);
        query.bindValue(":city", contacts.at(i)->city);
        query.bindValue(":birth_day", contacts.at(i)->birth_day);
        query.bindValue(":country", contacts.at(i)->country);
        query.bindValue(":list", contacts.at(i)->list);
        query.bindValue(":company", contacts.at(i)->company);
        query.exec();
    }
    db.commit();
}

void DatabaseManager::requestData(QString fieldSearched, QString fieldValue)
{
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

}

QList<QStringList> DatabaseManager::selectAll()
{
    QSqlQuery query;

    int nbCategory = selectDistinct("category");
    int nbCompany = selectDistinct("company");
    int nbList = selectDistinct("list");

    query.exec("SELECT * FROM contacts");

    if(!query.exec())
      qWarning() << "ERROR: " << query.lastError().text();
    QList<QStringList> list;

    while (query.next()) {

        QStringList item;
            for(int i = 0; i < 12; i++)
            {
                //qDebug() << __FUNCTION__ << __LINE__ << query.value(i);
                item.append(query.value(i).toString());
            }
            list.append(item);
        }
    emit addResults(list, nbCategory, nbCompany, nbList);
    return list;
}

bool DatabaseManager::createTable()
{

    db                      = QSqlDatabase::addDatabase("QSQLITE");
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir    dir(appDataLocation);
    if (!dir.exists()) {
        dir.mkdir(appDataLocation);
        qDebug() << __FUNCTION__ << __LINE__ << "mkdir" << appDataLocation;
    }
    QString dbPath = appDataLocation + "/testdb.db";
    qDebug() << "dbPath" << dbPath;
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qWarning() << "Unable to open db" << dbPath;
        return false;
    }
    qDebug() << __FUNCTION__ << __LINE__ << "creating table 'contacts'";
    QString tblFilesCreate = "CREATE TABLE IF NOT EXISTS contacts ("
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
                             ")";
    QSqlQuery query;
    query.exec(tblFilesCreate);
    if (query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    return true;

}
