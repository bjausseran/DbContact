#include "../BdDContactLibrary/bddcontactlibrary.h"
#include <QtTest>
#include <QCoreApplication>
#include "../BdDContactLibrary/databasemanager.h"
// add necessary includes here

class ContactTest : public QObject
{
    Q_OBJECT
    CsvManager *csvManager;
    DatabaseManager *dbManager;
    Contact *contact;
    QList<Contact*> contacts;
    int _testState=1;

public:
    ContactTest();
    ~ContactTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1(QStringList data);
    void test_case2();

};

ContactTest::ContactTest()
{

}

ContactTest::~ContactTest()
{

}

void ContactTest::initTestCase()
{

    dbManager =new DatabaseManager();

    QObject::connect(dbManager, &DatabaseManager::sendContactData, this, &ContactTest::test_case1);
    dbManager->ReadFile("../../DbContact/TestFolder/myFile4.csv");
    dbManager->requestData("id","1");


}

void ContactTest::cleanupTestCase()
{

}

void ContactTest::test_case1(QStringList requestList)
{
    QString string;

    if(_testState==4){
        for(int i=0; i<requestList.size(); i++)
        {
            string += requestList[i];
        }
        qDebug() <<__FUNCTION__ <<"TestExport "<< string;
        QCOMPARE(string, "7TestExport");
    }
    if(_testState==3){
        for(int i=0; i<requestList.size(); i++)
        {
            string += requestList[i];
        }
        qDebug() <<__FUNCTION__ <<"TestUpdate "<< string;
        QCOMPARE(string, "2Test76f29450-aeda-4144-b71f-3bed815596b1TestAllisTestWattenbergTestAllis.Wattenberg@yopmail.comTest7296985TestWorkToulouseTest1999-04-24FRTestBlacklistTestNVidia");
        _testState=4;
    }
    if(_testState==2){
        for(int i=0; i<requestList.size(); i++)
        {
            string += requestList[i];
        }
        qDebug() <<__FUNCTION__ <<"TestDelete "<< string;
        QCOMPARE(string, "");
        _testState=3;
        dbManager->updateData({"company"}, {"TestNVidia"}, {"country", "city"}, {"FR", "Toulouse"});
        dbManager->requestData("company","TestNVidia");
    }
    if(_testState==1){
        for(int i=0; i<requestList.size(); i++)
        {
            string += requestList[i];
        }
        qDebug() <<__FUNCTION__ <<"TestInsert "<< string;
        QCOMPARE(string, "1TestGUIDfirstnamelastnameemailtelcategorycitybirth_daycountrylistcompany");
        dbManager->deleteData("id","3");
        _testState=2;
        dbManager->requestData("id","3");
    }
}

void ContactTest::test_case2()
{
    csvManager =new CsvManager();
    contact =new Contact();

    contact->birth_day="TestExport";

    contacts.append(contact);

    csvManager->writeCsvFile(contacts,"../../DbContact/TestFolder/myFile5.csv");
    dbManager->ReadFile("../../DbContact/TestFolder/myFile5.csv");
    dbManager->requestData("birth_day","TestExport");




}

QTEST_MAIN(ContactTest)

#include "tst_contacttest.moc"
