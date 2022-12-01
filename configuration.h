#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QDir>

class Configuration : public QObject
{
    Q_OBJECT
public:
    Configuration(QObject *parent = nullptr);
    void WriteToConf(QString title, QVariant value);
    QVariant ReadFromConf(QString title);
private:
    QSettings* settings;
};

#endif // CONFIGURATION_H
