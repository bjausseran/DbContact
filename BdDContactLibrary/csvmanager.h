#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <QFile>
#include <QFuture>
#include <QObject>
#include "contact.h"
#include <QThread>

class BDDCONTACTLIBRARY_EXPORT CsvManager : public QObject
{
    Q_OBJECT

public:
    explicit CsvManager(QObject *parent = nullptr);
    QFuture<QStringList> readCsvfile(QString path);
    void writeCsvFile(Contact contacts[], QString fileName);
    void writeCsvFile(QList<Contact*> contacts, QString fileName);

signals:
    void fileRead(QStringList data);
    void fileWrote(QString file);
    void countDown(int count);

public slots:
    void onCsvRequested(Contact contacts[], QString fileName);
};

#endif // CSVMANAGER_H
