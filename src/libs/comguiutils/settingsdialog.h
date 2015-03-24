#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "comguiutils_global.h"
#include "ioptionspage.h"

#include <QList>
#include <QSet>
#include <QEventLoop>
#include <QDialog>

class QModelIndex;
class QSortFilterProxyModel;
class QStackedLayout;
class QLabel;
class QListView;
class QLineEdit;

namespace GuiUtils {

class Category;
class CategoryModel;

class COMGUIUTILS_EXPORT SettingsDialog : public QDialog
{
    Q_OBJECT

public:    
    static SettingsDialog *getSettingsDialog(Id app, QWidget *parent, Id initialCategory, Id initialPage);
    static bool showOptionsDialog(Id app, Id category, Id page, QWidget *parent);
    bool execDialog();
    virtual QSize sizeHint() const;

public slots:
    void done(int);

private slots:
    void accept();
    void reject();
    void apply();
    void currentChanged(const QModelIndex &current);
    void currentTabChanged(int);
    void filter(const QString &text);
    void ensureAllCategoryWidgets();

private:
    SettingsDialog(Id app, QWidget *parent);
    ~SettingsDialog();

    void createGui();
    void showCategory(int index);
    void showPage(Id categoryId, Id pageId);
    void updateEnabledTabs(Category *category, const QString &searchText);
    void ensureCategoryWidget(Category *category);
    void disconnectTabWidgets();    

    const QList<IOptionsPage *> m_pages;

    QSet<IOptionsPage *> m_visitedPages;
    QSortFilterProxyModel *m_proxyModel;
    CategoryModel *m_model;
    Id m_currentCategory;
    Id m_currentPage;
    QStackedLayout *m_stackedLayout;
    QLineEdit *m_filterLineEdit;
    QListView *m_categoryList;
    QLabel *m_headerLabel;
    Id m_app;
    bool m_running;
    bool m_applied;
    bool m_finished;
    QList<QEventLoop *> m_eventLoops;
};

}
#endif // SETTINGSDIALOG_H
