#ifndef COMMANDBUTTON_H
#define COMMANDBUTTON_H

#include "comactionmanager_global.h"

#include <comutils/id.h>

#include <QPointer>
#include <QString>
#include <QToolButton>

using namespace CommonUtils;

namespace GuiUtils {

class Command;

class COMACTIONMANAGER_EXPORT CommandButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QString toolTipBase READ toolTipBase WRITE setToolTipBase)
public:
    explicit CommandButton(QWidget *parent = nullptr);
    explicit CommandButton(Id id, QWidget *parent = nullptr);
    void setCommandId(Id id);
    QString toolTipBase() const;
    void setToolTipBase(const QString &toolTipBase);

private slots:
    void updateToolTip();

private:
    QPointer<Command> m_command;
    QString m_toolTipBase;
};

}

#endif // COMMANDBUTTON_H
