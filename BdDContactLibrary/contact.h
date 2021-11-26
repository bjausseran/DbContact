#ifndef CONTACT_H
#define CONTACT_H

#include <QObject>
#include "BdDContactLibrary_global.h"

/*
 * FIELDS :
 *
 * id,
 * GUID,
 * firstname,
 * lastname,
 * email,
 * tel,
 * category,
 * city,
 * birth_day,
 * country,
 * list,
 * company
 *
 * */

class BDDCONTACTLIBRARY_EXPORT Contact : public QObject
{
    Q_OBJECT
public:
    explicit Contact(QObject *parent = nullptr);
    int id;
    QString GUID;
    QString firstname;
    QString lastname;
    QString email;
    QString tel;
    QString category;
    QString city;
    QString birth_day;
    QString country;
    QString list;
    QString company;

};

#endif // CONTACT_H
