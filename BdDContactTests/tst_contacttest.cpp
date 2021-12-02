#include "../BdDContactLibrary/bddcontactlibrary.h"
#include <QtTest>
#include <QCoreApplication>
#include "../BdDContactLibrary/databasemanager.h"
// add necessary includes here

class ContactTest : public QObject
{
    Q_OBJECT
    DatabaseManager *dbManager;
    int _testState=1;

public:
    ContactTest();
    ~ContactTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1(QStringList data);


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
        dbManager->deleteData("id","1");
        _testState=2;
        dbManager->requestData("id","1");
    }
}

QTEST_MAIN(ContactTest)

#include "tst_contacttest.moc"
