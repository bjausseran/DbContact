#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

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


    QLabel *nbLineLabel;
    QLabel *nbCategoryLabel;
    QLabel *nbCompanyLabel;
    QLabel *nbListLabel;


    void addLine(QStringList values);
    void refreshUI();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setForm(QStringList values);
    void createTableLayout();
public slots:
    void onContactDataReceived(QStringList list);
    void addResults(QList<QStringList> values, int nbCategory, int nbCompany, int nbList);
    void onContactSelected(const QModelIndex &index);

signals:
    void requestContactList();
    void deleteContact(int id);
    void requestContactData(int id);
    void updateContact(QStringList fieldsSearched, QStringList fieldsValue, QStringList fieldsUpdated, QStringList newValues);
private slots:
    void on_update_btn_clicked();

    void on_delete_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
