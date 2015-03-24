#include "uiloader.h"

#include <QScriptContext>
#include <QScriptEngine>
#include <QFile>
#include <QDebug>

UiLoader::UiLoader(QObject *parent) : QUiLoader(parent)
{
}

UiLoader::~UiLoader()
{
    qDebug() << "delete Uiloader.";
}

QWidget *UiLoader::load(const QString &uifile, QWidget *parentWidget)
{
    if (!QFile::exists(uifile)) return nullptr;
    QFile file(uifile);
    if (!file.open(QFile::ReadOnly))
        return nullptr;
    QWidget* widget = nullptr;
    widget = QUiLoader::load(&file, parentWidget);
    file.close();
    return widget;
}


