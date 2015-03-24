#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "comactionmanager_global.h"
#include "command.h"

#include <comutils/id.h>

#include <QObject>
#include <QList>

class QAction;
class QSettings;
class QShortcut;
class QString;

namespace GuiUtils {

class ActionContainer;

class COMACTIONMANAGER_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    ActionManager(QWidget *parent);
    ~ActionManager();

    static ActionManager *instance();

    static ActionContainer *createMenu(Id id, QWidget* parent);
    static ActionContainer *createMenuBar(Id id);

    static Command *registerAction(QAction *action, Id id, const Context &context, QWidget* parent, bool scriptable = false);
    static Command *registerShortcut(QShortcut *shortcut, Id id, const Context &context, QWidget* parent, bool scriptable = false);

    static Command *command(Id id);
    static ActionContainer *actionContainer(Id id);

    static QList<Command *> commands();

    static void unregisterAction(QAction *action, Id id);
    static void unregisterShortcut(Id id);

    static void setPresentationModeEnabled(bool enabled);
    static bool isPresentationModeEnabled();

    static void initialize(Id app);
    static void saveSettings(const QString &idPrefix, QSettings *settings);
    static Context context();
    static void updateContexts(const Context& remove, const Context& add);

signals:
    void commandListChanged();
    void commandAdded(const QString &id);

private:
    void setContext(const Context &context);
};

} // namespace GuiUtils

#endif // ACTIONMANAGER_H
