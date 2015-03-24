#ifndef SCMAINWINDOW_H
#define SCMAINWINDOW_H

#include "comactionmanager_global.h"
#include "context.h"

#include <QMainWindow>

namespace GuiUtils {

class ActionContainer;
class ActionManager;

class COMACTIONMANAGER_EXPORT ScMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ScMainWindow(bool canMainApp, QWidget *parent = nullptr);
    void updateCurrentMainWindowContexts(const Context& remove, const Context& add);
    Context currentContext() const { return m_context; }
    ActionManager* actionManager() const { return m_actionManager; }
    ActionContainer* recentFileMenu() const { return m_recentFileMenu; }
    ActionContainer* themeViewMenu() const { return m_themeViewMenu; }
    ActionContainer* toolbarViewMenu() const { return m_toolbarViewMenu; }
    ActionContainer* docksViewMenu() const { return m_docksViewMenu; }

    virtual void delayInit();

    static QString recentFileOpenDirectory();

signals:
    void closed();

protected slots:
    virtual void slotOpenRecentFile() {}
    void slotViewtFullscreen();

private slots:
    void slotFileClearRecentFiles();
    void slotViewChangeTheme();
    void slotAboutToShowViewToolbarMenu();
    void slotViewToolbars(bool checked);
    void slotViewDocks(bool checked);
    void slotUpdateDockWindowList();

protected:
    void setCurrentFile(const QString& file);
    void updateRecentFiles();
    void readSettings();
    void writeSettings();

private:
    enum { MaxRecentFiles = 9 };
    Context                 m_context; //当前应用此主窗口代表的Context
    QAction*                m_recentFileActs[MaxRecentFiles];
    QString                 m_currentTheme;
    ActionContainer*        m_recentFileMenu = nullptr;
    ActionContainer*        m_themeViewMenu = nullptr;
    ActionContainer*        m_toolbarViewMenu = nullptr;
    ActionContainer*        m_docksViewMenu = nullptr;
    ActionManager*          m_actionManager = nullptr;
    bool                    m_canMainApp;
};

}

#endif // SCMAINWINDOW_H
