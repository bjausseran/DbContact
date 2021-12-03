#include "csvmanager.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent>
#include <QDirIterator>

CsvManager::CsvManager(QObject *parent) : QObject(parent)
{

}

QFuture<QStringList> CsvManager::readCsvfile(QString path)
{
    return QtConcurrent::run([this, path]()
        {

            qDebug() << __FUNCTION__ << __LINE__ << path;
            QFile file(path);

            qDebug() << __FUNCTION__ << __LINE__ << &file;

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                new QException();

            QStringList result;

            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                result.append(line);
                if(result.count() > 40000)
                {
                    QFuture<void> future = QtConcurrent::run(this, &CsvManager::fileRead, result);
                    result.clear();
                }
            }

            QFuture<void> future = QtConcurrent::run(this, &CsvManager::fileRead, result);
            return result;

    });
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

void CsvManager::writeCsvFile(QList<Contact*> contacts, QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);


auto count = 0;
    for(int i = 0; i < contacts.length(); i++)
    {
        out << contacts.value(i)->id
            << ","
            << contacts.value(i)->GUID
            << ","
            << contacts.value(i)->firstname
            << ","
            << contacts.value(i)->lastname
            << ","
            << contacts.value(i)->email
            << ","
            << contacts.value(i)->tel
            << ","
            << contacts.value(i)->category
            << ","
            << contacts.value(i)->city
            << ","
            << contacts.value(i)->birth_day
            << ","
            << contacts.value(i)->country
            << ","
            << contacts.value(i)->list
            << ","
            << contacts.value(i)->company
            << "\n";
        count++;

        emit countDown(((contacts.count() + count) * 100) / (contacts.count() * 2));
    }
    emit fileWrote(fileName);
}

void CsvManager::onCsvRequested(Contact contacts[], QString fileName)
{
    writeCsvFile(contacts, fileName);
}

