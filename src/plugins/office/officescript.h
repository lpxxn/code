#ifndef OFFICESCRIPT_H
#define OFFICESCRIPT_H

#include <QScriptExtensionPlugin>

class OfficeScript : public QScriptExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.peraglobal.SimCube.OfficeScriptPlugin")
public:
    OfficeScript();
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);

};

#endif // OFFICESCRIPT_H
