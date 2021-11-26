#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include "BdDContactLibrary_global.h"
#include "csvmanager.h"


class BDDCONTACTLIBRARY_EXPORT DatabaseManager : public QObject
{
    Q_OBJECT
    QString dbName = "";
    QSqlDatabase db;
    CsvManager *csv;

    bool createTable();

public:
    explicit DatabaseManager(QObject *parent = nullptr);

    void addData(QList<Contact*> contacts);
    void addData(Contact contacts[]);
    void requestData(QString fieldSearched, QString fieldValue);
    QList<QStringList> selectAll();
    void cleanDb();
    void deleteData(QString fieldSearched, QString fieldValue);
    void updateData(QString fieldSearched, QString fieldValue, QString fieldUpdated, QString newValue);
    void updateData(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);
    void ReadFile(QString path);

    int selectDistinct(QString field);

signals:
    void requestCsvCreation(Contact contacts[], QString filename);
    void sendContactData(QStringList list);
    void addResults(QList<QStringList> values, int nbCategory, int nbCompany, int nbList);
public slots:
    void onFileRead(QStringList list);
    void onUpdateRequest(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);


};


#endif // DATABASEMANAGER_H
