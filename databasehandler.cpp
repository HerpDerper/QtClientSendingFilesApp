#include "databasehandler.h"
#include <QDebug>

DataBaseHandler::DataBaseHandler(QObject* parent) : QObject(parent)
{

}

void DataBaseHandler::init()
{
    emit MoveToLog("Производиться инициализация базы данных");
    QString db_name = QApplication::applicationDirPath() + QDir::separator() + "database.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);
    if (db.open())
    {
        emit MoveToLog("База данных открыта");
    }
    else
    {
        emit MoveToLog(QString("Внутри базы данных произошли ошибка: %1").arg(db.lastError().text()));
    }
    QSqlQuery* query = new QSqlQuery(db);
    bool queryStatus = query->exec("create table if not exists content_of_files (id integer primary key autoincrement, content text not null)");
    if(queryStatus)
    {
        emit MoveToLog("Таблица для записи успешно создана");
    }
}

void DataBaseHandler::InsertFileToDataBase(QByteArray content)
{
    QByteArray start = "F0BC20FD59BB";
    QByteArray end = "1D73DF02A644";
    int initial_pos = -1;
    int end_pos = -1;
    while(true){
        if (content.contains(start)){
            int current = 0;
            for (int i = 0; i < content.count(); i++){
                QChar c = start.at(current);
                if (content.at(i) == c){
                    current++;
                }
                else if(current > 0){
                    current = 0;
                    i--;
                }
                else{
                    current = 0;
                }
                if (current == 11){
                    //F0BC20FD59B_B_
                    initial_pos = i+2;
                    break;
                }
            }
        }
        else{
            return;
        }
        if (content.contains(end) && initial_pos != -1){
            int current = 0;
            for (int i = initial_pos; i < content.count(); i++){
                QChar c = end.at(current);
                if (content.at(i) == c){
                    current++;
                    qDebug() << current;
                }
                else if(current > 0){
                    current = 0;
                    i--;
                }
                else{
                    current = 0;
                }
                if (current == 11){
                    //1D73DF02A64_4_
                    end_pos = i-10;
                    break;
                }
            }
        }
        else{
            return;
        }

        if (initial_pos > end_pos && initial_pos != end_pos && initial_pos != -1 && end_pos != -1){
            return;
        }

        QByteArray context = content.mid(initial_pos, end_pos-12);
        qDebug() << end_pos;
        content = content.remove(0, end_pos+12);
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("insert into content_of_files (content) values (:content)");
        QString str = context;
        query->bindValue(":content", str);
        if(query->exec())
        {
            emit MoveToLog("Запись успешно создана");
        }
        else
        {
            emit MoveToLog(QString("При создании записи произошла ошибка: %1").arg(query->lastError().text()));
        }

        initial_pos = -1;
        end_pos = -1;
        qDebug() << content;
    }








}
