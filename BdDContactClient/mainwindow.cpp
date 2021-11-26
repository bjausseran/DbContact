#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::addLine(QStringList values)
{
    int nbItem = tableWidget->rowCount();
    tableWidget->insertRow(nbItem);

    QTableWidgetItem *id = new QTableWidgetItem(values.at(0));
    tableWidget->setItem(nbItem, 0, id);

    QTableWidgetItem *name = new QTableWidgetItem(values.at(2));
    tableWidget->setItem(nbItem, 1, name);

    QTableWidgetItem *lastname = new QTableWidgetItem(values.at(3));
    tableWidget->setItem(nbItem, 2, lastname);

    QTableWidgetItem *category = new QTableWidgetItem(values.at(6));
    tableWidget->setItem(nbItem, 3, category);

    QTableWidgetItem *company = new QTableWidgetItem(values.at(11));
    tableWidget->setItem(nbItem, 4, company);
}

void MainWindow::refreshUI()
{
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    emit requestContactList();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tableWidget = ui->tableWidget;
    idLabel = ui->form_id_edit;
    guidLabel = ui->form_GUID_edit;
    firstNameLabel = ui->form_firstname_edit;
    lastNameLabel = ui->form_lastname_edit;
    emailLabel = ui->form_email_edit;
    telLabel = ui->form_phone_edit;
    categoryLabel = ui->form_category_edit;
    cityLabel = ui->form_city_edit;
    birth_dayLabel = ui->form_birthday_edit;
    countryLabel = ui->form_country_edit;
    listLabel = ui->form_list_edit;
    companyLabel = ui->form_company_edit;

    nbLineLabel = ui->nbLine_edit;
    nbCategoryLabel = ui->nbCategory_edit;
    nbCompanyLabel = ui->nbCompany_edit;
    nbListLabel = ui->nbList_edit;


    createTableLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setForm(QStringList values)
{
    qDebug() << __FUNCTION__ << __LINE__ << values.length();

    idLabel->setText(values.at(0));
    guidLabel->setText(values.at(1));
    firstNameLabel->setText(values.at(2));
    lastNameLabel->setText(values.at(3));
    emailLabel->setText(values.at(4));
    telLabel->setText(values.at(5));
    categoryLabel->setText(values.at(6));
    cityLabel->setText(values.at(7));
    birth_dayLabel->setText(values.at(8));
    countryLabel->setText(values.at(9));
    listLabel->setText(values.at(10));
    companyLabel->setText(values.at(11));
}

void MainWindow::onContactDataReceived(QStringList list)
{
    qDebug() << __FUNCTION__ << __LINE__ << list.length();

    setForm(list);
}

void MainWindow::createTableLayout()
{
    tableWidget->setColumnCount(5);
    tableWidget->setColumnWidth(0, 50);
    //tableWidget->setColumnWidth(1, 180);
    //connect( tableWidget, &QAbstractItemView::doubleClicked, this, &MainWindow::set);
    this->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(tableWidget, &QAbstractItemView::doubleClicked, this, &MainWindow::onContactSelected);

    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Firstname"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Lastname"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Category"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Company"));

}

void MainWindow::addResults(QList<QStringList> values, int nbCategory, int nbCompany, int nbList)
{
    for(int i = 0; i < values.length(); i++)
    {
        addLine(values.at(i));
    }
    nbLineLabel->setText(QString::number(values.length()));
    nbCategoryLabel->setText(QString::number(nbCategory));
    nbCompanyLabel->setText(QString::number(nbCompany));
    nbListLabel->setText(QString::number(nbList));

    emit requestContactData(tableWidget->item(0, 0)->text().toInt());
}

void MainWindow::onContactSelected(const QModelIndex &index)
{

    qDebug() << __FUNCTION__ << __LINE__ << "id = " << index.row();
    emit requestContactData(tableWidget->item(index.row(), 0)->text().toInt());

}


void MainWindow::on_update_btn_clicked()
{
    QStringList searchedFields = {"id"};
    QStringList searchedValues = {idLabel->text()};

    QStringList updatedFields = {"firstname", "lastname", "email", "tel", "category", "city", "birth_day", "country", "list", "company"};
    QStringList updatedValues = {firstNameLabel->text(), lastNameLabel->text(), emailLabel->text(), telLabel->text(), categoryLabel->text(), cityLabel->text(), birth_dayLabel->text(), countryLabel->text(), listLabel->text(), companyLabel->text()};

    emit updateContact(searchedFields, searchedValues, updatedFields, updatedValues);
    refreshUI();

}


void MainWindow::on_delete_btn_clicked()
{
    emit deleteContact(idLabel->text().toInt());
    refreshUI();
}

