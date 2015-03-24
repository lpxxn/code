#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include "comguiutils_global.h"
#include <QDialog>

namespace GuiUtils {

class COMGUIUTILS_EXPORT VersionDialog : public QDialog
{
    Q_OBJECT
public:
    VersionDialog(const QString &appVersion, QWidget *parent);

private:
    QString buildCompatibilityString();
    QString compilerString();
};

}

#endif // VERSIONDIALOG_H
