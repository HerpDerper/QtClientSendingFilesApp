#include "configuration.h"

Configuration::Configuration(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QApplication::applicationDirPath() + QDir::separator() + "settings.ini", QSettings::IniFormat);
}

void Configuration::WriteToConf(QString title, QVariant value)
{
    settings->setValue(title, value);
}

QVariant Configuration::ReadFromConf(QString title)
{
    return settings->value(title, "default");
}
