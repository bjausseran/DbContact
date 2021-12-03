#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtConcurrent>
#include "BdDContactLibrary_global.h"
#include "csvmanager.h"


class BDDCONTACTLIBRARY_EXPORT DatabaseManager : public QObject
{
    Q_OBJECT
    QString dbName = "";
    //QSqlDatabase db;
    CsvManager *csv;

    QSqlDatabase setupDatabase();
    void initQuery();
    void cleanDatabase(QSqlDatabase &db);
    QFuture<void> selectDataForExport(QStringList idList, QString path);
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    void addDataFromList(QList<Contact*> contacts);
    void requestData(QString fieldSearched, QString fieldValue);
    QFuture<QList<QStringList>> selectAll();
    void cleanDb(QSqlDatabase &db);
    void deleteData(QString fieldSearched, QString fieldValue);
    void updateData(QString fieldSearched, QString fieldValue, QString fieldUpdated, QString newValue);
    void updateData(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);
    void ReadFile(QString path);

    int selectDistinct(QString field);

    QFuture<QList<QStringList>> requestDataList(QString fieldSearched, QString fieldValue);
signals:
    void requestCsvCreation(Contact contacts[], QString filename);
    void sendContactData(QStringList list);
    void addResults(QList<QStringList> values);
    void addStats(int nbLine, int nbCategory, int nbCompany, int nbList);
    void countDown(int count);
public slots:
    void onFileRead(QStringList list);
    void onUpdateRequest(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);
    void onExportCsvRequest(QStringList idList, QString path);


};


#endif // DATABASEMANAGER_H
