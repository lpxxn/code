#ifndef COMDATAACTIONMANAGER_H
#define COMDATAACTIONMANAGER_H

#include "comdatagui_global.h"

#include <QObject>

namespace GuiUtils {
    class ActionContainer;
}

namespace ComDataGui {

class BookTable;

class COMDATAGUI_EXPORT ComDataActionManager : public QObject
{
    Q_OBJECT
public:
    static ComDataActionManager* instance(QObject *parent = nullptr);
    void setCurrentBookTable(BookTable* bt);
    BookTable* currentBookTable() const { return m_curTable; }

    void createEditActions(GuiUtils::ActionContainer* editMenu, QWidget* parent);
    void createTableActions(GuiUtils::ActionContainer* tableMenu, QWidget *parent);
    void createGraphActions(GuiUtils::ActionContainer* graphMenu, QWidget *parent);
    void createPlotActions(GuiUtils::ActionContainer* mplot, QWidget *parent);

signals:
    void graphUndo();
    void graphRedo();

private slots:
    void slotTableUndo();
    void slotTableRedo();
    void slotGraphUndo();
    void slotGraphRedo();
    void slotMoveColumn();
    void slotColumnSetAs();
    void slotAppendColumn();
    void slotInsertColumn();
    void slotMaskData();
    void slotHideColumn();
    void slotSortColumn();
    void slotSortTable();
    void slotFillNumber();
    void slotClearTable();
    void slotRemoveColumn();
    void slotAppendRow();
    void slotInsertRow();
    void slotToggleTableItemDisplay(bool display);
    void slotToggleTableDisplay(bool display);
    void slotUpdateToggleTableItemMenu();
    void slotUpdateToggleTableMenu();

private:
    ComDataActionManager(QObject* parent = nullptr);

private:
    BookTable* m_curTable = nullptr;
    static ComDataActionManager* stComDataActMng;
};

}

#endif // COMDATAACTIONMANAGER_H
