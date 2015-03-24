#ifndef UISCRIPT_H
#define UISCRIPT_H

#include <QScriptExtensionPlugin>

class QWidget;

class UIScript : public QScriptExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.peraglobal.SimCube.UIScriptPlugin")
public:
    UIScript();
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);

private slots:
    QWidget* uiInteractiveDelegate(QScriptEngine *engine, const QString &code);
};

#endif // UISCRIPT_H
