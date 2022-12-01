#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFile>
#include "databasehandler.h"
#include "configuration.h"
#include <QHostAddress>


const QString IP_ADRESS = "ip";
const QString PORT = "port";

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString REQUEST = "request";
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void LoadData();

    void BrowserClear();

    void AddToLog(QString text);

    void Connect();
    void DisconnectSocket();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    quint64 nextBlockSize;
    QByteArray socketArray;
    DataBaseHandler* db;
    Configuration* configuration;

    void readyRead();

    void ConnectToServer();
};
#endif // MAINWINDOW_H
