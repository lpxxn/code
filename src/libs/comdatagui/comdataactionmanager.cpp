#include "comdataactionmanager.h"
#include "constants.h"
#include "booktable.h"

#include <comactionmanager/actioncontainer.h>
#include <comactionmanager/actionmanager.h>
#include <comdata/column.h>
#include <comutils/utilsconstants.h>

#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QCoreApplication>

using namespace GuiUtils;
using namespace ComDataGui;

ComDataActionManager* ComDataActionManager::stComDataActMng = nullptr;

ComDataActionManager *ComDataActionManager::instance(QObject* parent)
{
    if (!stComDataActMng)
        stComDataActMng = new ComDataActionManager(parent);
    return stComDataActMng;
}

ComDataActionManager::ComDataActionManager(QObject* parent) : QObject(parent)
{
}

void ComDataActionManager::setCurrentBookTable(BookTable *bt)
{
    if (m_curTable == bt) return;
    m_curTable = bt;
}

void ComDataActionManager::createEditActions(ActionContainer *editMenu, QWidget *parent)
{
    Context worksheetContext(ComDataGui::Constants::C_VIZ_WORKSHEET);
    // Undo Action
    QIcon icon = QIcon::fromTheme(QStringLiteral("edit-undo"), QIcon(QLatin1String(ComDataGui::Constants::ICON_UNDO)));
    QAction* tmpAction = new QAction(icon, tr("&Undo"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotTableUndo()));
    tmpAction->setStatusTip(tr("Undo the last action"));
    Command* cmd = ActionManager::registerAction(tmpAction, CommonUtils::Constants::UNDO, worksheetContext, parent);
    cmd->setDefaultKeySequence(QKeySequence::Undo);
    cmd->setAttribute(Command::CA_Hide);
    cmd->setDescription(tr("Undo"));
    editMenu->addAction(cmd, ComDataGui::Constants::G_EDIT_UNDOREDO);

    // Redo Action
    icon = QIcon::fromTheme(QLatin1String("edit-redo"), QIcon(QLatin1String(ComDataGui::Constants::ICON_REDO)));
    tmpAction = new QAction(icon, tr("&Redo"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotTableRedo()));
    tmpAction->setStatusTip(tr("Redo the cancel action"));
    cmd = ActionManager::registerAction(tmpAction, CommonUtils::Constants::REDO, worksheetContext, parent);
    cmd->setDefaultKeySequence(QKeySequence::Redo);
    cmd->setAttribute(Command::CA_Hide);
    cmd->setDescription(tr("Redo"));
    editMenu->addAction(cmd, ComDataGui::Constants::G_EDIT_UNDOREDO);
}

void ComDataActionManager::createTableActions(ActionContainer *tableMenu, QWidget* parent)
{
    Context worksheetContext(ComDataGui::Constants::C_VIZ_WORKSHEET);

    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_MOVE);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_COLUMNSETAS);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_TOGGLE);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_APPENDINSERT);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_DATAMARK);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_SORT);
    tableMenu->appendGroup(ComDataGui::Constants::G_TABLE_FILL);

    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_COLUMNSETAS);
    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_TOGGLE);
    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_APPENDINSERT);
    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_DATAMARK);
    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_SORT);
    tableMenu->addSeparator(worksheetContext, parent, ComDataGui::Constants::G_TABLE_FILL);

    // table menu items
    //move to first
    QIcon icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNMOVFIRST));
    QAction* tmpAction  = new QAction(icon, tr("Move to First"), parent);
    tmpAction->setData(ComDataGui::Constants::MOVEFIRST);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMoveColumn()));
    Command* cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::MOVEFIRST, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_MOVE);

    //move to end
    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNMOVELAST));
    tmpAction  = new QAction(icon, tr("Move to End"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMoveColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::MOVEEND, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tmpAction->setData(ComDataGui::Constants::MOVEEND);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_MOVE);

    //move left
    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNMOVELEFT));
    tmpAction  = new QAction(icon, tr("Move Left"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMoveColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::MOVELEFT, worksheetContext, parent);
    tmpAction->setData(ComDataGui::Constants::MOVELEFT);
    cmd->setAttribute(Command::CA_Hide);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_MOVE);

    //move right
    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNMOVRIGHT));
    tmpAction  = new QAction(icon, tr("Move Right"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMoveColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::MOVERIGHT, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tmpAction->setData(ComDataGui::Constants::MOVERIGHT);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_MOVE);

    // Column Set Significance
    ActionContainer* mcolumnHeaderSet = ActionManager::createMenu(ComDataGui::Constants::M_COLUMNHEADERSETTING, parent);
    mcolumnHeaderSet->setOnAllDisabledBehavior(ActionContainer::Hide);
    mcolumnHeaderSet->appendGroup(ComDataGui::Constants::G_TABLE_COLUMNSETAS);
    mcolumnHeaderSet->menu()->setTitle(tr("Column Set As"));
    tableMenu->addMenu(mcolumnHeaderSet, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNX));
    tmpAction = new QAction(icon , tr("X"), parent);
    tmpAction->setData(ComData::ColumnBase::X);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::X, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNY));
    tmpAction = new QAction(icon, tr("Y"), parent);
    tmpAction->setData(ComData::ColumnBase::Y);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::Y, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNZ));
    tmpAction = new QAction(icon, tr("Z"), parent);
    tmpAction->setData(ComData::ColumnBase::Z);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::Z, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    mcolumnHeaderSet->menu()->addSeparator();

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNV));
    tmpAction = new QAction(icon , tr("V"), parent);
    tmpAction->setData(ComData::ColumnBase::V);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::V, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNVX));
    tmpAction = new QAction(icon , tr("VX"), parent);
    tmpAction->setData(ComData::ColumnBase::VX);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::VX, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNVY));
    tmpAction = new QAction(icon, tr("VY"), parent);
    tmpAction->setData(ComData::ColumnBase::VY);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::VY, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNVZ));
    tmpAction = new QAction(icon, tr("VZ"), parent);
    tmpAction->setData(ComData::ColumnBase::VZ);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::VZ, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    mcolumnHeaderSet->menu()->addSeparator();

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNLABEL));
    tmpAction = new QAction(icon, tr("Label"), parent);
    tmpAction->setData(ComData::ColumnBase::Label);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::LABEL, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNDISREGARD));
    tmpAction = new QAction(icon, tr("Disregard"), parent);
    tmpAction->setData(ComData::ColumnBase::Disregard);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::DISREGARD, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    mcolumnHeaderSet->menu()->addSeparator();

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNXERROR));
    tmpAction = new QAction(icon, tr("XError"), parent);
    tmpAction->setData(ComData::ColumnBase::XError);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::XERROR, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    icon = QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNYERROR));
    tmpAction = new QAction(icon, tr("YError"), parent);
    tmpAction->setData(ComData::ColumnBase::YError);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotColumnSetAs()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::YERROR, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mcolumnHeaderSet->addAction(cmd, ComDataGui::Constants::G_TABLE_COLUMNSETAS);

    // show ComplexTable Action
    ActionContainer* mshowComplexRow = ActionManager::createMenu(ComDataGui::Constants::M_SHOWCOMPLEXROW, parent);
    mshowComplexRow->setOnAllDisabledBehavior(ActionContainer::Hide);
    mshowComplexRow->appendGroup(ComDataGui::Constants::G_TABLE_TOGGLE);
    mshowComplexRow->menu()->setTitle(tr("Toggle Table Item Display"));
    tableMenu->addMenu(mshowComplexRow, ComDataGui::Constants::G_TABLE_TOGGLE);
    connect(mshowComplexRow->menu(), SIGNAL(aboutToShow()), SLOT(slotUpdateToggleTableItemMenu()));

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWLONGNAME, worksheetContext, parent);
    QAction* proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::TABLEROWLONGNAME);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWUNIT, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::TABLEROWUNIT);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWCOMMENT, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::TABLEROWCOMMENT);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWUSER, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::TABLEROWUSERFUNCTION);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWFILTER, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER));
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL), parent);
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWTHUMBNAILS, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    proxyact->setChecked(true);
    proxyact->setData(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableItemDisplay(bool)));
    mshowComplexRow->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    // Show WorkSheet --------------
    ActionContainer * mshowTable = ActionManager::createMenu(ComDataGui::Constants::M_WORKSHEETSHOW, parent);
    mshowTable->setOnAllDisabledBehavior(ActionContainer::Hide);
    mshowTable->appendGroup(ComDataGui::Constants::G_TABLE_TOGGLE);
    mshowTable->menu()->setTitle(tr("Toggle Table Display"));
    tableMenu->addMenu(mshowTable, ComDataGui::Constants::G_TABLE_TOGGLE);
    connect(mshowTable->menu(), SIGNAL(aboutToShow()), SLOT(slotUpdateToggleTableMenu()));

    tmpAction = new QAction(tr("Header Label"), parent);
    tmpAction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::WORKSHEETSHOWHEADERLABEL, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    //proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::WORKSHEETSHOWHEADERLABEL);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableDisplay(bool)));
    mshowTable->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(tr("Column Header"), parent);
    tmpAction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::WORKSHEETSHOWCOLUMNHEADER, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    //proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::WORKSHEETSHOWCOLUMNHEADER);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableDisplay(bool)));
    mshowTable->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(tr("Row Header"), parent);
    tmpAction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::WORKSHEETSHOWROWHEADER, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    //proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::WORKSHEETSHOWROWHEADER);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableDisplay(bool)));
    mshowTable->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);

    tmpAction = new QAction(tr("Grid"), parent);
    tmpAction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::WORKSHEETSHOWGRID, worksheetContext, parent);
    proxyact = cmd->action();
    proxyact->setCheckable(true);
    //proxyact->setChecked(true);
    proxyact->setData(ComDataGui::Constants::WORKSHEETSHOWGRID);
    connect(proxyact, SIGNAL(toggled(bool)), SLOT(slotToggleTableDisplay(bool)));
    mshowTable->addAction(cmd, ComDataGui::Constants::G_TABLE_TOGGLE);


    // Append Column Menu
    ActionContainer * appendColumnMenu = ActionManager::createMenu(ComDataGui::Constants::M_ADDCOLUMN, parent);
    appendColumnMenu->setOnAllDisabledBehavior(ActionContainer::Hide);
    tableMenu->addMenu(appendColumnMenu, ComDataGui::Constants::G_TABLE_APPENDINSERT);
    appendColumnMenu->menu()->setTitle((tr("Append Column")));
    appendColumnMenu->appendGroup(ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append StringColumn
    tmpAction  = new QAction(tr("String"), parent);
    tmpAction->setData(ComData::ColumnBase::String);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDSTRINGCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::String);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append IntegerColumn
    tmpAction  = new QAction(tr("Integer"), parent);
    tmpAction->setData(ComData::ColumnBase::Integer);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDINTEGERCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Integer);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append FloatColumn
    tmpAction  = new QAction(tr("Float"), parent);
    tmpAction->setData(ComData::ColumnBase::Float);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDFLOATCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Float);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append DoubleColum
    tmpAction  = new QAction(tr("Double"), parent);
    tmpAction->setData(ComData::ColumnBase::Double);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDDOUBLECOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Double);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append FloatComplexColumn
    tmpAction  = new QAction(tr("Float Complex"), parent);
    tmpAction->setData(ComData::ColumnBase::FloatComplex);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDFLOATCOMPLEXCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::FloatComplex);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append DoubleComplexColumn
    tmpAction  = new QAction(tr("Double Complex"), parent);
    tmpAction->setData(ComData::ColumnBase::DoubleComplex);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDDOUBLECOMPLEXCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::DoubleComplex);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Append DateTimeColumn
    tmpAction  = new QAction(tr("DateTime"), parent);
    tmpAction->setData(ComData::ColumnBase::DateTime);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ADDDATETIMECOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::DateTime);
    appendColumnMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert Column Menu
    ActionContainer * insertColumn = ActionManager::createMenu(ComDataGui::Constants::M_INSERTCOLUMN, parent);
    insertColumn->setOnAllDisabledBehavior(ActionContainer::Hide);
    tableMenu->addMenu(insertColumn, ComDataGui::Constants::G_TABLE_APPENDINSERT);
    insertColumn->menu()->setTitle((tr("Insert Column")));
    insertColumn->appendGroup(ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert StringColumn
    tmpAction  = new QAction(tr("String"), parent);
    tmpAction->setData(ComData::ColumnBase::String);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTSTRINGCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::String);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert IntegerColumn
    tmpAction  = new QAction(tr("Integer"), parent);
    tmpAction->setData(ComData::ColumnBase::Integer);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTINTEGERCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Integer);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert FloatColumn
    tmpAction  = new QAction(tr("Float"), parent);
    tmpAction->setData(ComData::ColumnBase::Float);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTFLOATCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Float);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert DoubleColumn
    tmpAction  = new QAction(tr("Double"), parent);
    tmpAction->setData(ComData::ColumnBase::Double);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTDOUBLECOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::Double);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert FloatComplexColumn
    tmpAction  = new QAction(tr("Float Complex"), parent);
    tmpAction->setData(ComData::ColumnBase::FloatComplex);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTFLOATCOMPLEXCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::FloatComplex);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert DoubleComplexColumn
    tmpAction  = new QAction(tr("Double Complex"), parent);
    tmpAction->setData(ComData::ColumnBase::DoubleComplex);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTDOUBLECOMPLEXCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::DoubleComplex);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // Insert DateTimeColumn
    tmpAction  = new QAction(tr("DateTime"), parent);
    tmpAction->setData(ComData::ColumnBase::DateTime);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTDATETIMECOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(ComData::ColumnBase::ColumnType::DateTime);
    insertColumn->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // delete column
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_DELETECOL)), tr("Delete Column"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::DELETECOLUMN, worksheetContext, parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotRemoveColumn()));
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // append row
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_APPEND_ROW)), tr("Append Row"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::APPENDROW, worksheetContext, parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotAppendRow()));
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    // insert row
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_INSERT_ROW)), tr("Insert Row"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::INSERTROW, worksheetContext, parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotInsertRow()));
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    //clear table
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_CLEAR)), tr("Clear Table"), parent);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotClearTable()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::CLEARTABLE, worksheetContext, parent);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_APPENDINSERT);

    //mark
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_MARK)), tr("Mark"), parent);
    tmpAction->setData(ComDataGui::Constants::MARK);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMaskData()));
    cmd->setAttribute(Command::CA_Hide);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::MARK, worksheetContext, parent);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_DATAMARK);

    //unmark
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_UNMARK)), tr("UnMark"), parent);
    tmpAction->setData(ComDataGui::Constants::UNMARK);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotMaskData()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::UNMARK, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_DATAMARK);

    // show column
    tmpAction  = new QAction(tr("Show Column"), parent);
    tmpAction->setData(ComDataGui::Constants::SHOWCOLUMN);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotHideColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::SHOWCOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_DATAMARK);

    // hide column
    tmpAction  = new QAction(tr("Hide Column"), parent);
    tmpAction->setData(ComDataGui::Constants::HIDECOLUMN);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotHideColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::HIDECOLUMN, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_DATAMARK);

    // column Sort
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ASC_COLUMN)), tr("Sort Column by Asc"), parent);
    tmpAction->setData(ComDataGui::Constants::COLUMNSORTASC);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotSortColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::COLUMNSORTASC, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(Qt::SortOrder::AscendingOrder);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_SORT);

    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_DESC_COLUMN)), tr("Sort Column by Desc"), parent);
    tmpAction->setData(ComDataGui::Constants::COLUMNSORTDESC);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotSortColumn()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::COLUMNSORTDESC, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(Qt::SortOrder::DescendingOrder);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_SORT);

    //Table Sort
    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ASC_TABLE)), tr("Sort Table by Asc"), parent);
    tmpAction->setData(ComDataGui::Constants::BOOKTABLESORTASC);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotSortTable()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::BOOKTABLESORTASC, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(Qt::SortOrder::AscendingOrder);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_SORT);

    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_DESC_TABLE)), tr("Sort Table by Desc"), parent);
    tmpAction->setData(ComDataGui::Constants::BOOKTABLESORTDESC);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotSortTable()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::BOOKTABLESORTDESC, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    cmd->action()->setData(Qt::SortOrder::DescendingOrder);
    tableMenu->addAction(cmd, ComDataGui::Constants::G_TABLE_SORT);

    // fill Number
    ActionContainer * colNW = ActionManager::createMenu(ComDataGui::Constants::M_COLUMN_FILLNUMWITH, parent);
    colNW->setOnAllDisabledBehavior(ActionContainer::Hide);
    tableMenu->addMenu(colNW, ComDataGui::Constants::G_TABLE_FILL);
    colNW->appendGroup(ComDataGui::Constants::G_TABLE_FILL);
    colNW->menu()->setTitle(tr("Fill Number With"));

    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ROWNUMBER)), tr("Row Numbers"),parent);
    tmpAction->setData(ComDataGui::Constants::ROWNUMBER);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotFillNumber()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::ROWNUMBER, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    colNW->addAction(cmd, ComDataGui::Constants::G_TABLE_FILL);

    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_UNIFORMRANDOMNUMBERS)), tr("Uniform Random Numbers"), parent);
    tmpAction->setData(ComDataGui::Constants::UNIFORMRANDOMNUMBERS);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotFillNumber()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::UNIFORMRANDOMNUMBERS, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    colNW->addAction(cmd, ComDataGui::Constants::G_TABLE_FILL);

    tmpAction  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_NORMALRANDOMNUMBERS)), tr("Normal Random Numbers"), parent);
    tmpAction->setData(ComDataGui::Constants::NORMALRANDOMNUMBERS);
    connect(tmpAction, SIGNAL(triggered()), SLOT(slotFillNumber()));
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::NORMALRANDOMNUMBERS, worksheetContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    colNW->addAction(cmd, ComDataGui::Constants::G_TABLE_FILL);
}

void ComDataActionManager::createGraphActions(ActionContainer *graphMenu, QWidget *parent)
{
    Context graphContext(ComDataGui::Constants::C_VIZ_GRAPH);
    graphMenu->appendGroup(ComDataGui::Constants::G_GRAPH_LAYER);

    // Undo Action
    QAction* tmpAction = new QAction(tr("&Undo"), parent);
    tmpAction->setStatusTip(tr("Undo the last action"));
    Command* cmd = ActionManager::registerAction(tmpAction, CommonUtils::Constants::UNDO, graphContext, parent);
    cmd->setDefaultKeySequence(QKeySequence::Undo);
    cmd->setAttribute(Command::CA_Hide);
    cmd->setDescription(tr("Undo"));
    connect(tmpAction, SIGNAL(triggered()), SIGNAL(graphUndo()));

    // Redo Action
    tmpAction = new QAction(tr("&Redo"), parent);
    tmpAction->setStatusTip(tr("Redo the cancel action"));
    cmd = ActionManager::registerAction(tmpAction, CommonUtils::Constants::REDO, graphContext, parent);
    cmd->setDefaultKeySequence(QKeySequence::Redo);
    cmd->setAttribute(Command::CA_Hide);
    cmd->setDescription(tr("Redo"));
    connect(tmpAction, SIGNAL(triggered()), SIGNAL(graphRedo()));

    // MARK, GRAPH MENU
#if 0
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_NEWLEGEND)), tr("New Legend"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHNEWLEGEND, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphTest()));

    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_UPDATELEGEND)), tr("Update Legend"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHUPDATELEGEND, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);

    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_FITLAYERTOLEGEND)), tr("Fit Layer To Legend"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPFITLAYERTOGRAPH, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);

    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_EXCHANGEXYAXISES)), tr("Exchange XY Axises"), parent);
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHEXCHANGEXYAXIS, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
#endif
    // add 'Export Image'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_EXPORTIMAGE)), tr("Export Image"), parent);
    tmpAction->setData("exportImage");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHEXPORTIMAGE, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));

    // add 'Tile Graphs'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_TILEGRAPHS)), tr("Tile Graphs"), parent);
    tmpAction->setData("tileGraphs");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHTILEGRAPHS, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));
    graphMenu->menu()->addSeparator();

    // add 'Zoom In'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMIN)), tr("Zoom In"), parent);
    tmpAction->setData("zoomIn");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMIN, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));

    // add 'Zoom Out'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMOUT)), tr("Zoom Out"), parent);
    tmpAction->setData("zoomOut");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMOUT, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));
    graphMenu->menu()->addSeparator();

    // add 'Zoom In Horizontally'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMINHOR)), tr("Zoom In Horizontally"), parent);
    tmpAction->setData("zoomInHorizontally");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMINHOR, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));

    // add 'Zoom Out Horizontally'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMOUTHOR)), tr("Zoom Out Horizontally"), parent);
    tmpAction->setData("zoomOutHorizontally");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMOUTHOR, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));
    graphMenu->menu()->addSeparator();

    // add 'Zoom In Vertically'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMINVER)), tr("Zoom In Vertically"), parent);
    tmpAction->setData("zoomInVertically");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMINVER, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));

    // add 'Zoom Out Vertically'
    tmpAction = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_ZOOMOUTVER)), tr("Zoom Out Vertically"), parent);
    tmpAction->setData("zoomOutVertically");
    cmd = ActionManager::registerAction(tmpAction, ComDataGui::Constants::GRAPHZOOMOUTVER, graphContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    graphMenu->addAction(cmd, ComDataGui::Constants::G_GRAPH_LAYER);
    connect(tmpAction, SIGNAL(triggered()), parent, SLOT(slotGraphFunc()));
}

void ComDataActionManager::createPlotActions(ActionContainer *mplot, QWidget *parent)
{
    Context tableContext(ComDataGui::Constants::C_VIZ_WORKSHEET);

    mplot->appendGroup(ComDataGui::Constants::G_PLOT_LINE);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_AREA);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_BUBBLE);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_COLUMN);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_BAR);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_SURFACE);
    mplot->appendGroup(ComDataGui::Constants::G_PLOT_PIE);

    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_AREA);
    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_BUBBLE);
    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_COLUMN);
    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_BAR);
    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_SURFACE);
    mplot->addSeparator(tableContext, parent, ComDataGui::Constants::G_PLOT_PIE);

    QAction* action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_LINE)), tr("Line"), parent);
    GuiUtils::Command* cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTLINE, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_LINE);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_HORIZONTALSTEP)), tr("HorizontalStep"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTHORIZONTALSTEP, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_LINE);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_VERTICALSTEP)), tr("VerticalStep"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTVERTICALSTEP, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_LINE);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_SPLINECONNECTED)), tr("SplineConnected"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTSPLINECONNECTED, tableContext, parent);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_LINE);

    // plot area
    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_AREA)), tr("Area"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTAREA, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_AREA);

    // plot bubble
    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_BUBBLE)), tr("Bubble"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTBUBBLE, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_BUBBLE);

#ifdef RESERVE
    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_STACKAREA)), tr("Stack Area"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTSTACKAREA, tableContext, parent);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_AREA);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_FILLAREA)), tr("Fill Area"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTFILLAREA, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_AREA);
#endif

    // plot cbp
    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMN)), tr("Column"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTCOLUMN, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_COLUMN);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_COLUMNPLUSLABEL)), tr("Column Plus Label"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTCOLUMNPLUSLABEL, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_COLUMN);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_STACKCOLUMN)), tr("Stack Column"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTSTACKCOLUMN, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_COLUMN);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_BAR)), tr("Bar"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTBAR, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_BAR);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_BARPLUSLABEL)), tr("Bar Plus Label"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTBARPLUSLABEL, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_BAR);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_STACKBAR)), tr("Stack Bar"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTSTACKBAR, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_BAR);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_FLOATINGCOLUMN)), tr("Floating Column"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTFLOATINGCOLUMN, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_COLUMN);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_FLOATINGBAR)), tr("Floating Bar"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOTFLOATINGBAR, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_BAR);

    // test code
    action = new QAction(tr("Surface"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOT3DSURFACE, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_SURFACE);

//#ifdef RESERVE
    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_PIE3D)), tr("3D Color Pie Chart"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOT3DCOLORPIECHART, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_PIE);

    action  = new QAction(QIcon(QLatin1String(ComDataGui::Constants::ICON_PIE2D)), tr("2D BW Pie Chart"), parent);
    cmd = ActionManager::registerAction(action, ComDataGui::Constants::PLOT2DBWPIECHART, tableContext, parent);
    cmd->setAttribute(Command::CA_Hide);
    mplot->addAction(cmd, ComDataGui::Constants::G_PLOT_PIE);
//#endif
}

void ComDataActionManager::slotTableUndo()
{
    if (m_curTable)
        m_curTable->undo();
}

void ComDataActionManager::slotTableRedo()
{
    if (m_curTable)
        m_curTable->redo();
}

void ComDataActionManager::slotGraphUndo()
{
}

void ComDataActionManager::slotGraphRedo()
{
}

/*!
 * \brief 移动列
 */
void ComDataActionManager::slotMoveColumn()
{
    if (!m_curTable)
        return;

    QList<int> selectedCols = m_curTable->selectedColumnIndex();
    QAction* action = qobject_cast<QAction*>(sender());
    QString str(action->data().toString());

    for (const int i : selectedCols) {
        int to = 0;
        if (str == ComDataGui::Constants::MOVEFIRST) {
            m_curTable->moveColumn(i, to);
        } else if (str == ComDataGui::Constants::MOVEEND) {
            to = m_curTable->columnsCount() - 1;
            m_curTable->moveColumn(i, to);
        } else if (str == ComDataGui::Constants::MOVELEFT) {
            if (i > 0) {
                to = i - 1;
                m_curTable->swapColumn(i, to);
            }
        } else if(str == ComDataGui::Constants::MOVERIGHT) {
            if (i < m_curTable->columnsCount() - 1) {
                to = i + 1;
                m_curTable->swapColumn(i, to);
            } else
                to = i;
        }
        m_curTable->setCurrentColumn(to);
    }
}

void ComDataActionManager::slotColumnSetAs()
{
    if (!m_curTable)
        return;

    QList<int> selectedCols = m_curTable->selectedColumnIndex();
    QAction* action = qobject_cast<QAction*>(sender());

    if (selectedCols.size() == 1)
        m_curTable->setColumnSignificance(selectedCols.first(), ComData::ColumnBase::Significance(action->data().toInt()));
    else
        m_curTable->setColumnsSignificance(selectedCols, ComData::ColumnBase::Significance(action->data().toInt()));
}

void ComDataActionManager::slotAppendColumn()
{
    if (!m_curTable)
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    m_curTable->appendColumn(ComData::ColumnBase::ColumnType(action->data().toInt()));
}

void ComDataActionManager::slotInsertColumn()
{
    if (!m_curTable)
        return;

    QList<int> selectedCols = m_curTable->selectedColumnIndex();
    if (selectedCols.isEmpty())
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    m_curTable->insertColumn(selectedCols[0], ComData::ColumnBase::ColumnType(action->data().toInt()));
}

void ComDataActionManager::slotMaskData()
{
    if (!m_curTable)
        return;
    QAction* action = qobject_cast<QAction*>(sender());
    m_curTable->setSelectedCellsMarked(action->data().toString() == ComDataGui::Constants::MARK);
}

void ComDataActionManager::slotHideColumn()
{
    if (!m_curTable)
        return;
    QAction* action = qobject_cast<QAction*>(sender());
    if (action->data().toString() == ComDataGui::Constants::SHOWCOLUMN) {
        QList<int> hiddenCols;
        for (int i = 0; i < m_curTable->columnsCount(); ++i)
            if (!m_curTable->isColumnVisible(i))
                hiddenCols.append(i);

        m_curTable->setColumnsVisible(hiddenCols, true);
    } else {
        //所有已选中的列
        QList<int> selectedCols = m_curTable->selectedColumnIndex();
        m_curTable->setColumnsVisible(selectedCols, false);
    }
}

void ComDataActionManager::slotSortColumn()
{
    if (!m_curTable)
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    QList<int> selectedCols = m_curTable->selectedColumnIndex();

    if (action->data().toString() == ComDataGui::Constants::COLUMNSORTASC) {
        for (int i : selectedCols)
            m_curTable->sortColumn(i, Qt::AscendingOrder);
    } else {
        for (int i : selectedCols)
            m_curTable->sortColumn(i, Qt::DescendingOrder);
    }
}

void ComDataActionManager::slotSortTable()
{
    if (!m_curTable)
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    QList<int> selectedCols = m_curTable->selectedColumnIndex();
    Qt::SortOrder so = (action->data().toString() == ComDataGui::Constants::BOOKTABLESORTASC ?
                            Qt::AscendingOrder : Qt::DescendingOrder);
    m_curTable->sortTable(selectedCols.first(), so);
}

void ComDataActionManager::slotFillNumber()
{
    if (!m_curTable)
        return;
    QAction* action = qobject_cast<QAction*>(sender());
    const QString temp = action->data().toString();
    if (temp == ComDataGui::Constants::ROWNUMBER)
        m_curTable->fillRowNumbers();
    else if (temp == ComDataGui::Constants::UNIFORMRANDOMNUMBERS)
        m_curTable->fillUniformRandom();
    else
        m_curTable->fillRandom();
}

void ComDataActionManager::slotClearTable()
{
    if (!m_curTable)
        return;
    m_curTable->clearContents();
}

void ComDataActionManager::slotRemoveColumn()
{
    if (!m_curTable)
        return;

    QList<int> selectedCols = m_curTable->selectedColumnIndex();
    m_curTable->removeColumns(selectedCols);
}

void ComDataActionManager::slotAppendRow()
{
    if (!m_curTable)
        return;

    m_curTable->appendRow();
}

void ComDataActionManager::slotInsertRow()
{
    if (!m_curTable)
        return;
    m_curTable->insertRow(m_curTable->selectedDataRow());
}

void ComDataActionManager::slotToggleTableItemDisplay(bool display)
{
    if (!m_curTable)
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    m_curTable->setTableRowVisible(action->text(), display);
}

void ComDataActionManager::slotToggleTableDisplay(bool display)
{
    if (!m_curTable)
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    m_curTable->setTableElementVisible(action->data().toString(), display);
}

void ComDataActionManager::slotUpdateToggleTableItemMenu()
{
    if (!m_curTable)
        return;
    m_curTable->updateTableItemMenu();
}

void ComDataActionManager::slotUpdateToggleTableMenu()
{
    if (!m_curTable)
        return;
    m_curTable->updateTableElemMenu();
}
