#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <QFile>
#include <QObject>
#include "contact.h"

class CsvManager : public QObject
{
    Q_OBJECT
public:
    explicit CsvManager(QObject *parent = nullptr);
    void readCsvfile(QString path);
    void writeCsvFile(Contact contacts[], QString fileName);

signals:
    void fileRead(QStringList data);
    void fileWrote(QString file);

public slots:
    void onCsvRequested(Contact contacts[], QString fileName);
};

#endif // CSVMANAGER_H
