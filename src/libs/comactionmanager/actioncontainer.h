#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "comactionmanager_global.h"
#include "context.h"

#include <QObject>

class QMenu;
class QMenuBar;
class QAction;

namespace GuiUtils {

class Command;

class COMACTIONMANAGER_EXPORT ActionContainer : public QObject
{
    Q_OBJECT

public:
    enum OnAllDisabledBehavior {
        Disable,
        Hide,
        Show
    };

    virtual void setOnAllDisabledBehavior(OnAllDisabledBehavior behavior) = 0;
    virtual ActionContainer::OnAllDisabledBehavior onAllDisabledBehavior() const = 0;

    virtual Id id() const = 0;

    virtual QMenu *menu() const = 0;
    virtual QMenuBar *menuBar() const = 0;

    virtual QAction *insertLocation(Id group) const = 0;
    virtual void appendGroup(Id group) = 0;
    virtual bool insertGroup(Id before, Id group) = 0;
    virtual void addAction(Command *action, Id group = Id()) = 0;
    virtual void addMenu(ActionContainer *menu, Id group = Id()) = 0;
    virtual void addMenu(ActionContainer *before, ActionContainer *menu, Id group = Id()) = 0;
    virtual Command *addSeparator(const Context &context, QWidget* parent, Id group = Id(), QAction **outSeparator = nullptr) = 0;
    virtual void clear() = 0;
    virtual QList<Command*> commandsInGroup(Id groupId) = 0;
};

} // namespace GuiUtils

#endif // ACTIONCONTAINER_H
