#ifndef UILOADER_H
#define UILOADER_H

#include <QUiLoader>
#include <QScriptable>

class UiLoader : public QUiLoader,
                 public QScriptable
{
    Q_OBJECT
public:
    explicit UiLoader(QObject *parent = nullptr);
    ~UiLoader();

    Q_INVOKABLE QWidget* load(const QString& uifile, QWidget* parentWidget);

};

#endif // UILOADER_H
