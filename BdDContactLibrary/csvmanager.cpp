#include "csvmanager.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

CsvManager::CsvManager(QObject *parent) : QObject(parent)
{

}

void CsvManager::readCsvfile(QString path)
{

    qDebug() << __FUNCTION__ << __LINE__ << path;
    QFile file(path);

    qDebug() << __FUNCTION__ << __LINE__ << &file;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList result;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        result.append(line);
    //qDebug() << __FUNCTION__ << __LINE__ << line;
    }

    emit fileRead(result);
}

void CsvManager::writeCsvFile(Contact contacts[], QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);


    int lenght = sizeof(contacts)/sizeof(contacts[0]);

    for(int i = 0; i < lenght; i++)
    {
        out << contacts[i].id
            << ","
            << contacts[i].GUID
            << ","
            << contacts[i].firstname
            << ","
            << contacts[i].lastname
            << ","
            << contacts[i].email
            << ","
            << contacts[i].tel
            << ","
            << contacts[i].category
            << ","
            << contacts[i].city
            << ","
            << contacts[i].birth_day
            << ","
            << contacts[i].country
            << ","
            << contacts[i].list
            << ","
            << contacts[i].company
            << "\n";
    }
    emit fileWrote(fileName);
}

void CsvManager::onCsvRequested(Contact contacts[], QString fileName)
{
    writeCsvFile(contacts, fileName);
}

