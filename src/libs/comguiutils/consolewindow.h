#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include "comguiutils_global.h"

#include <QWidget>
#include <QProcess>

class QPlainTextEdit;
class QPushButton;
class QLabel;
class QToolBar;
class QAction;
class QMenu;
class QLineEdit;
class QTreeView;
class QFileSystemModel;
class QComboBox;
class QTabWidget;
class ConsoleProcess;
class InfoDialog;
class LaunchDialog;

namespace ConsoleWindowUtil
{
    class FindBar;
    class ProcessTab;
}

class COMGUIUTILS_EXPORT ConsoleWindow : public QWidget
{
    Q_OBJECT
    enum InfoLevel
    {
        Normal,
        Success,
        Warning,
        Error
    };

public:
    explicit ConsoleWindow(QWidget *parent = 0);

    void addProcess(ConsoleProcess* process);

private slots:
    void clearOutput();
    void clearData();
    void showAllOutput();
    void findText();
    void showWorkDir();
    void showInputDir();
    void showOutputDir();
    void showInfo();
    void onFindText();
    void openDir();
    void saveAsFile();
    void launchProcess();
    void onDirMenuAboutToShow();
    void onTabCloseRequested(int index);
    void onRemoveTab();
    void onCurrentTabChanged(int index);

private:
    void createUi();
    QAction* createAction(const QString& text, const char* slotfunc = 0, bool enabled = true);
    QAction* createAction(const QString& text, const QIcon& icon, const char* slotfunc = 0);
    static void locateFile(const QString& file);
    void createClearMenu();
    void createDirMenu();
    void enableTerminateAction(bool enable);
    void updateActions(bool enable);
    ConsoleWindowUtil::ProcessTab* currentTab() const;

private:

    bool m_enableStdin : 1;
    bool m_enableStdout : 1;
    bool m_enableStderr : 1;

    ConsoleProcess* m_process;
    QAction* m_clearAct;
    QAction* m_updateAct;
    QAction* m_findAct;
    QAction* m_saveasAct;
    QAction* m_infoAct;
    QAction* m_filterAct;
    QAction* m_dirAct;
    QAction* m_openDirAct;
    QAction* m_launchAct;
    QMenu* m_clearMenu;
    QAction* m_clearOutput;
    QAction* m_clearData;
    QMenu* m_dirMenu;
    QAction* m_workdirAct;
    QAction* m_inputdirAct;
    QAction* m_outputdirAct;

    QTabWidget* m_processMainTab;
    QLabel* m_filePathLabel;

    ConsoleWindowUtil::FindBar* m_findBar;
    InfoDialog* m_infoDialog;
    LaunchDialog* m_launchDialog;

    friend class ConsoleWindowUtil::ProcessTab;
};

#endif // CONSOLEWINDOW_H
