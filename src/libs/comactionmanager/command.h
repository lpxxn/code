#ifndef COMMAND_H
#define COMMAND_H

#include "comactionmanager_global.h"

#include <comutils/id.h>

#include <QObject>

class QAction;
class QShortcut;
class QKeySequence;

using namespace CommonUtils;

namespace GuiUtils {

class Context;

#ifdef Q_OS_MAC
enum { UseMacShortcuts = 1 };
#else
enum { UseMacShortcuts = 0 };
#endif

class COMACTIONMANAGER_EXPORT Command : public QObject
{
    Q_OBJECT
public:
    enum CommandAttribute {
        CA_Hide = 1,
        CA_UpdateText = 2,
        CA_UpdateIcon = 4,
        CA_NonConfigurable = 8
    };
    Q_DECLARE_FLAGS(CommandAttributes, CommandAttribute)

    virtual void setDefaultKeySequence(const QKeySequence &key) = 0;
    virtual QKeySequence defaultKeySequence() const = 0;
    virtual QKeySequence keySequence() const = 0;    
    virtual void setDescription(const QString &text) = 0;
    virtual QString description() const = 0;

    virtual Id id() const = 0;

    virtual QAction *action() const = 0;
    virtual QShortcut *shortcut() const = 0;
    virtual Context context() const = 0;

    virtual void setAttribute(CommandAttribute attr) = 0;
    virtual void removeAttribute(CommandAttribute attr) = 0;
    virtual bool hasAttribute(CommandAttribute attr) const = 0;

    virtual bool isActive() const = 0;

    virtual void setKeySequence(const QKeySequence &key) = 0;
    virtual QString stringWithAppendedShortcut(const QString &str) const = 0;

    virtual bool isScriptable() const = 0;
    virtual bool isScriptable(const Context &) const = 0;

signals:
    void keySequenceChanged();
    void activeStateChanged();
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(GuiUtils::Command::CommandAttributes)

#endif // COMMAND_H
