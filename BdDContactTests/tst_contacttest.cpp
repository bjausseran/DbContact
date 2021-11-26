#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class ContactTest : public QObject
{
    Q_OBJECT

public:
    ContactTest();
    ~ContactTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

ContactTest::ContactTest()
{

}

ContactTest::~ContactTest()
{

}

void ContactTest::initTestCase()
{

}

void ContactTest::cleanupTestCase()
{

}

void ContactTest::test_case1()
{

}

QTEST_MAIN(ContactTest)

#include "tst_contacttest.moc"
