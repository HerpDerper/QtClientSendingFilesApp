#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configuration = new Configuration();

    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::BrowserClear);
    connect(ui->btnloadFile, &QPushButton::clicked, this, &MainWindow::LoadData);
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::Connect);
    db = new DataBaseHandler(this);
    db->init();
    connect(db, &DataBaseHandler::MoveToLog, this, &MainWindow::AddToLog);
    db->init();
    socket = new QTcpSocket();
    ConnectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);
    if (nextBlockSize == 0) {
        if ( socket->bytesAvailable() < (int)sizeof(quint64) )
            return;
        in >> nextBlockSize;
    }
    if (socket->bytesAvailable() < nextBlockSize)
        return;

    QByteArray file;
    in >> file;
    QFile* qfile = new QFile(file);
    QTextStream stream (&file, QIODevice::ReadOnly);

    if (ui->writeOnly->checkState() == Qt::CheckState::Checked)
        ui->textBrowser->append(stream.readAll());
    nextBlockSize = 0;
    emit AddToLog(QString("Были приняты данным размером %1").arg(file.size()));
    db->InsertFileToDataBase(file);
}


void MainWindow::AddToLog(QString text)
{
    ui->textBrowser->append(text);
}

void MainWindow::ConnectToServer()
{
    int port = configuration->ReadFromConf(PORT) != "default"?configuration->ReadFromConf(PORT).toInt() : 120;
    QString ip = configuration->ReadFromConf(IP_ADRESS) != "default"? configuration->ReadFromConf(IP_ADRESS).toString() : "127.0.0.1";
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    socket->connectToHost(QHostAddress(ip), port);
    socket->open(QIODevice::ReadWrite);
    nextBlockSize = 0;
    socket->waitForConnected(3000);
    if (socket->state() == QTcpSocket::SocketState::ConnectedState)
        emit AddToLog("Вы подключились к серверу");
    else
        emit AddToLog("Подлкючение к серверу отсутствует");
}

void MainWindow::LoadData()
{
    if (socket->state() != QTcpSocket::SocketState::ConnectedState){
        emit AddToLog("Подлкючение к серверу отсутствует");
        return;
    }
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << quint64(0) << REQUEST;
    out.device()->seek(0);
    out << quint64(data.size() - sizeof(quint64));
    socket->write(data);
    socket->waitForBytesWritten();
    emit AddToLog("Запрос на получение данных был успешно отправлен");
}

void MainWindow::BrowserClear()
{
    ui->textBrowser->clear();
}

void MainWindow::Connect()
{
    DisconnectSocket();
    ConnectToServer();
}

void MainWindow::DisconnectSocket()
{
    socket->disconnect();
}
