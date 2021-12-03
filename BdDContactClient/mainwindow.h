#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QStringList fields = {"id", "GUID", "firstname", "lastname", "email", "tel", "category", "city", "birth_day", "country", "list", "company"};


    QTableWidget *tableWidget;
    QLabel *idLabel;
    QLabel *guidLabel;
    QLineEdit *firstNameLabel;
    QLineEdit *lastNameLabel;
    QLineEdit *emailLabel;
    QLineEdit *telLabel;
    QLineEdit *categoryLabel;
    QLineEdit *cityLabel;
    QLineEdit *birth_dayLabel;
    QLineEdit *countryLabel;
    QLineEdit *listLabel;
    QLineEdit *companyLabel;

    QLineEdit *csvFolderEdit;

    QLineEdit *lookupField;
    QLineEdit *lookupValue;

    QLineEdit *csvExportPath;


    QLabel *nbLineLabel;
    QLabel *nbCategoryLabel;
    QLabel *nbCompanyLabel;
    QLabel *nbListLabel;


    void addLine(QStringList values);
    void addLines(QList<QStringList> values);
    void refreshUI();
    bool checkField(QString field);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setForm(QStringList values);
    void createTableLayout();
public slots:
    void onContactDataReceived(QStringList list);
    void addResults(QList<QStringList> values);
    void addStats(int nbLine, int nbCategory, int nbCompany, int nbList);
    void onContactSelected(const QModelIndex &index);

signals:
    void filterData(QString field, QString value);
    void requestContactList();
    void loadCsv(QString path);
    void exportCsv(QStringList idList, QString path);
    void deleteContact(int id);
    void requestContactData(int id);
    void updateContact(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);
private slots:
    void on_update_btn_clicked();

    void on_delete_btn_clicked();

    void on_updateList_btn_clicked();

    void on_loadCsv_btn_clicked();

    void on_filter_btn_clicked();

    void on_exportCsv_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
