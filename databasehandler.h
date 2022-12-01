#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QDir>


class DataBaseHandler : public QObject
{
    Q_OBJECT
public:
    DataBaseHandler(QObject* parent);
public slots:
    void init();
    void InsertFileToDataBase(QByteArray packet);

signals:
    void MoveToLog(QString text);

private:
    QSqlDatabase db;

};

#endif // DATABASEHANDLER_H
