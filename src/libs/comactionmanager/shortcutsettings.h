#ifndef COMMANDMAPPINGS_H
#define COMMANDMAPPINGS_H

#include "comactionmanager_global.h"

#include <comguiutils/ioptionspage.h>

#include <QObject>
#include <QKeySequence>

class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QKeyEvent;

namespace GuiUtils {

namespace Ui { class ShortcutSettings; }

class Command;
class ShortcutItem;

class COMACTIONMANAGER_EXPORT ShortcutSettings : public GuiUtils::IOptionsPage
{
    Q_OBJECT
public:
    ShortcutSettings(Id globalId, QObject *parent = nullptr);

protected:
    // IOptionsPage
    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();
    bool matches(const QString &s) const;

    bool filter(const QString &filterString, QTreeWidgetItem *item);
    bool eventFilter(QObject *o, QEvent *e);

    // access to m_page
    QTreeWidget *commandList() const;
    QLineEdit *targetEdit() const;
    QString filterText() const;
    void setModified(QTreeWidgetItem *item, bool modified);

private slots:
    void commandChanged(QTreeWidgetItem *current);
    void filterChanged(const QString &f);
    void targetIdentifierChanged();
    void resetTargetIdentifier();
    void initialize();
    void defaultAction();

private:
    void setKeySequence(const QKeySequence &key);
    void clear();
    void removeTargetIdentifier();

    void handleKeyEvent(QKeyEvent *e);
    int translateModifiers(Qt::KeyboardModifiers state, const QString &text);
    void markPossibleCollisions(ShortcutItem *item);
    void resetCollisionMarker(ShortcutItem *item);
    void resetCollisionMarkers();

private:
    Ui::ShortcutSettings *m_page;
    QList<ShortcutItem *> m_scitems;
    int m_key[4], m_keyNum;
    Id  m_globalId;
    QString m_searchKeywords;
    bool m_initialized;
};

} // namespace Core

#endif // COMMANDMAPPINGS_H
