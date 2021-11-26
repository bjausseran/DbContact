#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    DatabaseManager *dbMana = new DatabaseManager();

    QObject::connect(&w, &MainWindow::requestContactList, &w, [dbMana]{dbMana->selectAll();;});
    QObject::connect(dbMana, &DatabaseManager::addResults, &w, &MainWindow::addResults);

    QObject::connect(&w, &MainWindow::requestContactData, dbMana, [dbMana](int id){dbMana->requestData("id",QString::number(id));});
    QObject::connect(dbMana, &DatabaseManager::sendContactData, &w, &MainWindow::onContactDataReceived);

    QObject::connect(&w, &MainWindow::updateContact, dbMana, &DatabaseManager::onUpdateRequest);

    QObject::connect(&w, &MainWindow::deleteContact, dbMana, [dbMana](int id){dbMana->deleteData("id",QString::number(id));});

    //____________________________________________________________________
    //                       Read file, import in DB
    //dbMana->ReadFile("C:/CSVs/myFile0.csv");


    //____________________________________________________________________
    //                       Update all contact with ynov as company
    //dbMana->deleteData("company", "Facebook");


    //____________________________________________________________________
    //                       Update all contact with ynov as company
    //dbMana->updateData({"company"}, {"Ynov"}, {"country", "city"}, {"FR", "Toulouse"});

    //____________________________________________________________________
    //                       Display every contact in the base
    dbMana->selectAll();

    //____________________________________________________________________
    //                       Clean out DB
    //dbMana->cleanDb();

    return a.exec();

}
