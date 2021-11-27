#include "../BdDContactLibrary/bddcontactlibrary.h"
#include <QtTest>
#include <QCoreApplication>
#include "../BdDContactLibrary/databasemanager.h"
// add necessary includes here

class ContactTest : public QObject
{
    Q_OBJECT
    DatabaseManager *dbManager;

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
    for(int i=0; i<requestList.size(); i++)
       {
        string += requestList[i];
        }
    qDebug() <<__FUNCTION__ << string;
    QCOMPARE(string, "1TestGUIDfirstnamelastnameemailtelcategorycitybirth_daycountrylistcompany");
}

QTEST_MAIN(ContactTest)

#include "tst_contacttest.moc"
