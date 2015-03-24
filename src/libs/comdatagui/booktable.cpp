#include "booktable.h"
#include "detail/datamodel.h"
#include "detail/complextableview.h"
#include "detail/complexmodel.h"
#include "detail/datatableview.h"
#include "detail/bookundocommand.h"
#include "detail/delta.h"
#include "detail/comparator.h"
#include "detail/exporthandler.h"
#include "detail/booktableundomanager.h"

#include <comdata/table.h>
#include <comactionmanager/actioncontainer.h>
#include <comactionmanager/actionmanager.h>
#include <comdatagui/constants.h>
#include <comutils/settingsmanager.h>
#include <vizcore/vizconstants.h>

#include <QXlsx/xlsxworksheet.h>

#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>
#include <QTableView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QTime>
#include <QAbstractButton>
#include <QWidget>
#include <QUndoCommand>
#include <QDebug>
#include <QSettings>
#include <QScriptEngine>
#include <QScriptValue>
#include <QFileDialog>
#include <QThread>
#include <QApplication>

using namespace ComData;
using namespace GuiUtils;

namespace ComDataGui
{

class BookTablePrivate
{
public:
    explicit BookTablePrivate(BookTable* q);
    ~BookTablePrivate();

    void setComplexModel(ComplexModel* complexModel);
    void setDataModel(DataModel* dataModel);

    DataModel* dataModel() const;
    ComplexModel* complexModel() const;

    void setHorizonHeaderVisible(bool visible);
    bool isVerticalHeaderVisible();
    void setVerticalHeaderVisible(bool visible);

    void updateGeo(bool useHeight = false, bool useOnePixel = true);
    void adjustHeaderWidth();
    void updateIndex(TableViewBase* from, TableViewBase* to, TableModelBase* toModel,
                     int row, int column, int lastRow);
    int complexTableHeight();
    QSize size() const;
    void updateMinSize();
    void updateHorizontalLabels();
    void setCurrentColumnSignificance(ComData::ColumnBase::Significance sig);
    void populate();
    void setTableRowVisible(const QString &rowName, bool visible);
    bool isTableRowVisible(const QString& rowName);
    void setTableElementVisible(const QString& elem, bool show);
    void blockTableSignals(bool block);
    void emitDataChangedSignals();

    bool write(QXlsx::Worksheet* sheet);

    DataTableView *dataView() const;
    ComplexTableView* complexView() const;

    void setUndoable(bool yes);
    bool isUndoable() const;

    BookTable* q_ptr;
    ComData::Table* table = nullptr;
    int verticalHeaderWidth = 0;
    int m_complexTableHeight = 0;
    int columnWidth;
    bool isFirstShown = true;
    bool isAutoRelease = false;
    bool isCtrlPressed = false;
    bool m_isUndoable = true;
    DataModel* m_dataModel = nullptr;
    ComplexModel* m_complexModel = nullptr;
    ComplexTableView* complexTable = nullptr;
    DataTableView* dataTable = nullptr;
    QMenu* contextMenu = nullptr;
    QAbstractButton *complexCornerButton = nullptr;
    Qt::SortOrder horizontalSortOrder;
    ColumnPropertyData columnPropertyData;
    BookTableUndoManager* undoManager;
    QMap<int, ColumnPropertyData> columnPropDataMap;
};

BookTablePrivate::BookTablePrivate(BookTable *q) : q_ptr(q)
{
    complexTable = new ComplexTableView(q_ptr);
    complexTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    complexTable->setFrameShape(QFrame::NoFrame);
    complexTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    complexTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    complexCornerButton = complexTable->findChild<QAbstractButton *>();
    QHeaderView* horizonHeader = complexTable->horizontalHeader();
    horizontalSortOrder = Qt::AscendingOrder;

    q_ptr->connect(horizonHeader, SIGNAL(sectionDoubleClicked(int)), q_ptr, SLOT(onHorizontalHeaderDoubleClicked(int)));
    complexCornerButton->disconnect();
    dataTable = new DataTableView(q_ptr);
    dataTable->verticalHeader()->setDefaultAlignment(Qt::AlignRight|Qt::AlignVCenter);
    dataTable->setCornerButtonEnabled(false);
    dataTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    dataTable->setFrameShape(QFrame::NoFrame);
    dataTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dataTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dataTable->horizontalHeader()->hide();

    m_dataModel = new DataModel(q_ptr);
    m_dataModel->setTable(table);
    setDataModel(m_dataModel);

    m_complexModel = new ComplexModel(q_ptr);
    m_complexModel->setTable(table);
    m_complexModel->setHorizontalHeaderLabels(table != nullptr ? table->titles() : QStringList());
    setComplexModel(m_complexModel);

    undoManager = new BookTableUndoManager(q_ptr, q_ptr);
    q_ptr->connect(q_ptr, SIGNAL(addCommand(DeltaBase*)), undoManager, SLOT(addCommand(DeltaBase*)));
    q_ptr->connect(q_ptr, SIGNAL(addCommand(DeltaDataList)), undoManager, SLOT(addCommand(DeltaDataList)));
    q_ptr->connect(m_dataModel, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), q_ptr, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
    q_ptr->connect(complexTable, SIGNAL(horizontalHeaderClicked(int)), q_ptr, SLOT(onHorizontalHeaderClicked(int)));
    q_ptr->connect(complexTable, SIGNAL(resetColumn(QModelIndex)), q_ptr, SLOT(onResetColumn(QModelIndex)));
    q_ptr->connect(complexTable, SIGNAL(filterOut(QModelIndex, FilterData)), q_ptr, SLOT(onFilterOut(QModelIndex, FilterData)));
    q_ptr->connect(complexTable, SIGNAL(evaluate(QModelIndex,QString)), q_ptr, SLOT(onEvaluate(QModelIndex,QString)));
    q_ptr->connect(m_dataModel, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), complexTable, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
    q_ptr->connect(complexCornerButton,SIGNAL(clicked()),q_ptr,SLOT(onSetDataTableSelectAll()));
    q_ptr->connect(complexTable, SIGNAL(dataChanged(QModelIndex,QString,QVariant)), q_ptr, SLOT(onDataChanged(QModelIndex,QString,QVariant)));
    q_ptr->connect(complexTable->horizontalScrollBar(), SIGNAL(valueChanged(int)), dataTable->horizontalScrollBar(), SLOT(setValue(int)));
    q_ptr->connect(complexTable, SIGNAL(doubleClicked(QModelIndex)), q_ptr, SLOT(onComplexIndexDoubleClicked(QModelIndex)));
    q_ptr->connect(dataTable->horizontalScrollBar(), SIGNAL(valueChanged(int)), complexTable->horizontalScrollBar(), SLOT(setValue(int)));
    q_ptr->connect(complexTable->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), q_ptr, SLOT(updateSectionWidth(int,int,int)));
    q_ptr->connect(complexTable->horizontalHeader(), SIGNAL(sectionPressed(int)), q_ptr, SLOT(onSectionPressed(int)));
    q_ptr->connect(dataTable->verticalHeader(), SIGNAL(geometriesChanged()), q_ptr, SLOT(onDataHeaderGeoChanged()));
    q_ptr->connect(complexTable->verticalHeader(), SIGNAL(geometriesChanged()), q_ptr, SLOT(onComplexHeaderGeoChanged()));
    q_ptr->connect(dataTable, SIGNAL(clicked(QModelIndex)), q_ptr, SLOT(onDataCellClicked(QModelIndex)));
    q_ptr->connect(complexTable, SIGNAL(clicked(QModelIndex)), q_ptr, SLOT(onComplexCellClicked(QModelIndex)));
    q_ptr->connect(complexTable->verticalHeader(), SIGNAL(sectionResized(int, int, int)), q_ptr, SLOT(onComplexHeaderSectionResized(int, int, int)));
    q_ptr->connect(dataTable->verticalHeader(), SIGNAL(sectionPressed(int)), q_ptr, SLOT(onDataSectionPressed(int)));
    q_ptr->connect(complexTable->verticalHeader(), SIGNAL(sectionPressed(int)), q_ptr, SLOT(onComplexSectionPressed(int)));
    q_ptr->connect(m_complexModel, SIGNAL(rowVisible(QString, bool)), q_ptr, SLOT(onRowVisible(QString, bool)));

    q_ptr->connect(complexTable, SIGNAL(moveOutFromColumn(int)), q_ptr, SLOT(onMoveOutOfComplexTable(int)));
    q_ptr->connect(dataTable, SIGNAL(moveOutFromColumn(int)), q_ptr, SLOT(onMoveOutOfDataTable(int)));

    q_ptr->connect(dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), q_ptr, SLOT(onVBarValueChanged_DataTable(int)));

    q_ptr->connect(complexTable, SIGNAL(customClearSelection()), q_ptr, SLOT(onComplexTableClearSelection()));
    q_ptr->connect(dataTable, SIGNAL(customClearSelection()), q_ptr, SLOT(onDataTableClearSelection()));

    q_ptr->connect(complexTable->horizontalHeader(), SIGNAL(sectionEntered(int)), dataTable->horizontalHeader(), SIGNAL(sectionEntered(int)));
    q_ptr->connect(complexTable->horizontalHeader(), SIGNAL(sectionPressed(int)), dataTable->horizontalHeader(), SIGNAL(sectionPressed(int)));

    q_ptr->connect(complexTable, SIGNAL(pressed()), q_ptr, SLOT(onComplexTablePressed()));
    q_ptr->connect(dataTable, SIGNAL(pressed()), q_ptr, SLOT(onDataTablePressed()));
    q_ptr->connect(dataTable, SIGNAL(dataCommitted(int,QModelIndex,QVariant,QVariant,QVariant)),
                   q_ptr, SLOT(onDataCommitted(int,QModelIndex,QVariant,QVariant,QVariant)));
    q_ptr->connect(complexTable, SIGNAL(dataCommitted(int,QModelIndex,QVariant,QVariant,QVariant)),
                   q_ptr, SLOT(onDataCommitted(int,QModelIndex,QVariant,QVariant,QVariant)));
    q_ptr->connect(complexTable, SIGNAL(updateMinSize()), q_ptr, SLOT(updateMinSize()));

    m_complexTableHeight = complexTableHeight();
}

BookTablePrivate::~BookTablePrivate()
{
    delete m_dataModel;
    delete m_complexModel;
    delete complexTable;
    delete dataTable;
    delete contextMenu;

    if (isAutoRelease)
        delete table;
}

void BookTablePrivate::setComplexModel(ComplexModel *complexModel)
{
    if (complexTable->model() != complexModel) {
        complexModel->disconnect();

        complexTable->setModel(complexModel);
        q_ptr->connect(complexModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                       q_ptr, SLOT(onComplexDataInserted(QModelIndex,int,int)));
        q_ptr->connect(complexModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                       q_ptr, SLOT(onComplexDataRemoved(QModelIndex,int,int)));

//        q_ptr->connect(complexModel, SIGNAL(updateIndex(QModelIndex, int)), complexTable, SLOT(updateIndex(QModelIndex, int)));
        emit complexModel->layoutChanged();
    }
}

void BookTablePrivate::setDataModel(DataModel *dataModel)
{
    dataTable->setModel(dataModel);
    emit dataModel->layoutChanged();
}

DataModel* BookTablePrivate::dataModel() const
{
    return qobject_cast<DataModel*>(dataTable->model());
}

ComplexModel* BookTablePrivate::complexModel() const
{
    return qobject_cast<ComplexModel*>(complexTable->model());
}

void BookTablePrivate::setHorizonHeaderVisible(bool visible)
{
    int hei = 0;
    for (int i = 0; i < complexTable->verticalHeader()->count(); i++)
        hei += complexTable->verticalHeader()->sectionSize(i);

    if(visible)
        m_complexTableHeight = hei;
    else
        m_complexTableHeight = hei - complexTable->horizontalHeader()->height();
    complexTable->horizontalHeader()->setVisible(visible);
    m_complexTableHeight = complexTableHeight();
    updateGeo(true);
}

bool BookTablePrivate::isVerticalHeaderVisible()
{
    return complexTable->verticalHeader()->isVisible();
}

void BookTablePrivate::setVerticalHeaderVisible(bool isvisible)
{
    complexTable->verticalHeader()->setVisible(isvisible);
    dataTable->verticalHeader()->setVisible(isvisible);
}

void BookTablePrivate::updateGeo(bool useHeight, bool useOnePixel)
{
    complexTable->setFixedWidth(q_ptr->width());
    dataTable->setFixedWidth(q_ptr->width());
    complexTable->move(0,0);

    if (useHeight) {
        complexTable->setGeometry(0, 0, q_ptr->width(), complexTableHeight());
        dataTable->setFixedHeight(q_ptr->height() - complexTableHeight());

        if (dataTable->model() != NULL && dataTable->model()->rowCount() == 0)
            dataTable->move(0, complexTableHeight());
        else
            dataTable->move(0, complexTableHeight() - (useOnePixel ? 1 : 0));
    } else {
        complexTable->setGeometry(0, 0, q_ptr->width(), complexTable->height() +
                                        complexTable->verticalScrollBar()->maximum());
        dataTable->setFixedHeight(q_ptr->height() - complexTable->height());

        if (dataTable->model() != NULL && dataTable->model()->rowCount() == 0)
            dataTable->move(0, complexTable->height() +
                                 complexTable->verticalScrollBar()->maximum());
        else
            dataTable->move(0, complexTable->height() +
                               complexTable->verticalScrollBar()->maximum() - (useOnePixel ? 1 : 0));
    }
}

void BookTablePrivate::adjustHeaderWidth()
{
    int complexHeaderWidth = complexTable->verticalHeader()->width();
    int dataHeaderWidth = dataTable->verticalHeader()->width();
    int maxHeaderWidth = qMax(complexHeaderWidth, dataHeaderWidth);
    complexTable->verticalHeader()->setFixedWidth(maxHeaderWidth);
    dataTable->verticalHeader()->setFixedWidth(maxHeaderWidth);
}

void BookTablePrivate::updateIndex(TableViewBase *from, TableViewBase *to, TableModelBase *toModel, int row, int column, int lastRow)
{
    from->selectionModel()->clearSelection();
    from->resetLastRow();
    QModelIndex index = toModel->createIndex(row, column);
    to->setFocus();
    to->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    to->setLastRow(lastRow);

    // potential qt bug
    qDebug() << "has selection: " << from->selectionModel()->hasSelection();
}

int BookTablePrivate::complexTableHeight()
{
    int compTableHeight = 0;
    for (int i = 0; i < complexTable->verticalHeader()->count(); i++) {
        if (!complexTable->isRowHidden(i))
            compTableHeight += complexTable->verticalHeader()->sectionSize(i);
    }
    compTableHeight += complexTable->horizontalHeader()->height();
    return compTableHeight;
}

QSize BookTablePrivate::size() const
{
    auto computeSize = [](QTableView* tv)-> QSize {
        int w = 0, h = 0;
        for (int i = 0, n = tv->verticalHeader()->count(); i < n; ++i)
            if (!tv->isRowHidden(i))
                h += tv->verticalHeader()->sectionSize(i);
        h += tv->horizontalHeader()->height();

        for (int i = 0, n = tv->horizontalHeader()->count(); i < n; ++i)
            if (!tv->isColumnHidden(i))
                w += tv->horizontalHeader()->sectionSize(i);
        w += tv->verticalHeader()->width();
        return QSize(w, h);
    };
    const QSize sz1 = computeSize(complexTable);
    const QSize sz2 = computeSize(dataTable);
    return QSize(qMax(sz1.width(), sz2.width()), sz1.height() + sz2.height());
}

void BookTablePrivate::updateMinSize()
{
    const QString wrapperName = q_ptr->parent()->metaObject()->className();
    if (wrapperName.compare("QWidget", Qt::CaseInsensitive) != 0)
        return;

    int w = 0;
    int h = 0;

    w = complexTable->verticalHeader()->width();
    for (int i = 0; i < complexTable->model()->columnCount(); ++i)
        w += complexTable->columnWidth(i);

    h = complexTable->height();

    for (int i = 0; i < dataTable->model()->rowCount(); ++i)
        h += dataTable->rowHeight(i);

    // call this function will incur weird behavior on BookTable
    qDebug() << __FUNCTION__ << " now it's time to set the minimu size!";
    q_ptr->setMinimumSize(QSize(w, h));
}

void BookTablePrivate::updateHorizontalLabels()
{
    QStringList hLabels;
    for (int i = 0; i < table->size(); ++i)
        hLabels += table->at(i)->title();

    m_complexModel->setHorizontalHeaderLabels(hLabels);
}

static inline QString sig2str(ColumnBase::Significance sig)
{
    QString str;
    switch (sig) {
    case ComData::ColumnBase::Significance::X:
        str = QString("(%1)").arg("X");
        break;
    case ComData::ColumnBase::Significance::Y:
        str = QString("(%1)").arg("Y");
        break;
    case ComData::ColumnBase::Significance::Label:
        str = QString("(%1)").arg("L");
        break;
    case ComData::ColumnBase::Significance::Z:
        str = QString("(%1)").arg("Z");
        break;
    case ComData::ColumnBase::Significance::VX:
        str = QString("(%1)").arg("VX");
        break;
    case ComData::ColumnBase::Significance::VY:
        str = QString("(%1)").arg("VY");
        break;
    case ComData::ColumnBase::Significance::VZ:
        str = QString("(%1)").arg("VZ");
        break;
    case ComData::ColumnBase::Significance::V:
        str = QString("(%1)").arg("V");
        break;
    case ComData::ColumnBase::Significance::Disregard:
        str = "";
        break;
    case ComData::ColumnBase::Significance::XError:
        str = QString("(%1)").arg("XError");
        break;
    case ComData::ColumnBase::Significance::YError:
        str = QString("(%1)").arg("YError");
        break;
    default:
        Q_ASSERT(0);
        break;
    }
    return str;
}

void BookTablePrivate::setCurrentColumnSignificance(ColumnBase::Significance sig)
{
    QString str = sig2str(sig);

    //所有已选中的列
    QModelIndexList modelIndexList = dataTable->selectionModel()->selectedColumns();

    //所有的标头
    QStringList qHeaderTitleList = m_complexModel->horizontalHeaderLabels();

    for (const QModelIndex &modelIndex : modelIndexList) {
        QString strTitle = qHeaderTitleList.at(modelIndex.column());
        strTitle = strTitle.left(strTitle.indexOf("("));
        strTitle += str;
        qHeaderTitleList.replace(modelIndex.column(), strTitle);
        dataModel()->setSignificance(modelIndex.column(), sig);
    }
    m_complexModel->setHorizontalHeaderLabels(qHeaderTitleList);
}

void BookTablePrivate::populate()
{
    ComData::FloatColumn* xCol = new ComData::FloatColumn(table);
    xCol->setName("A");
    xCol->setSignificance(ColumnBase::X);

    ComData::FloatColumn* y1Col = new ComData::FloatColumn(table);
    y1Col->setName("B");
    y1Col->setSignificance(ColumnBase::Y);

    ComData::FloatColumn* y2Col = new ComData::FloatColumn(table);
    y2Col->setName("C");
    y2Col->setSignificance(ColumnBase::Y);

    if (table) {
        table->append(QSharedPointer<ColumnBase>(xCol));
        table->append(QSharedPointer<ColumnBase>(y1Col));
        table->append(QSharedPointer<ColumnBase>(y2Col));
    } else {
        delete xCol;
        delete y1Col;
        delete y2Col;
    }
}

void BookTablePrivate::setTableRowVisible(const QString &rowName, bool visible)
{
    if (visible)
        complexTable->showRow(m_complexModel->verticalHeaderLabels().indexOf(rowName));
    else
        complexTable->hideRow(m_complexModel->verticalHeaderLabels().indexOf(rowName));

    emit complexTable->model()->layoutChanged();
}

bool BookTablePrivate::isTableRowVisible(const QString &rowName)
{
    bool visible = !complexTable->isRowHidden(m_complexModel->verticalHeaderLabels().indexOf(rowName));
    return visible;
}

void BookTablePrivate::setTableElementVisible(const QString& elem, bool show)
{
    if (elem == ComDataGui::Constants::WORKSHEETSHOWHEADERLABEL)
        complexModel()->setVerticalLabelVisible(show);
    else if (elem ==  ComDataGui::Constants::WORKSHEETSHOWCOLUMNHEADER)
        setHorizonHeaderVisible(show);
    else if (elem ==  VizCore::Constants::WORKSHEETSHOWROWHEADER)
        setVerticalHeaderVisible(show);
    else if (elem ==  VizCore::Constants::WORKSHEETSHOWCOLUMNLINE)
        q_ptr->setShowGrid(show);
}

void BookTablePrivate::blockTableSignals(bool block)
{
    for (int i = 0; i < table->size(); ++i)
        table->at(i)->blockSignals(block);
}

void BookTablePrivate::emitDataChangedSignals()
{
    for (int i = 0; i < table->size(); ++i)
        emit table->at(i)->dataChanged();
}

bool BookTablePrivate::write(QXlsx::Worksheet *sheet)
{
    if (!sheet)
        return false;

    int row = 0;
    // write long name
    ++row;
    for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = table->at(colIndex);
        sheet->write(row, colIndex + 1, colData->longName());
    }

    // write unit
    ++row;
    for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = table->at(colIndex);
        sheet->write(row, colIndex + 1, colData->unit());
    }

    // write comment
    ++row;
    for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = table->at(colIndex);
        sheet->write(row, colIndex + 1, colData->comment());
    }

    // write data
    for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = table->at(colIndex);
        for (int rowIndex = 0; rowIndex < colData->size(); ++rowIndex) {
            sheet->write(row + 1 + rowIndex, colIndex + 1, colData->cell(rowIndex));
        }
    }

    return true;
}

DataTableView* BookTablePrivate::dataView() const
{
    return dataTable;
}

ComplexTableView *BookTablePrivate::complexView() const
{
    return complexTable;
}

void BookTablePrivate::setUndoable(bool yes)
{
    if (m_isUndoable != yes)
        m_isUndoable = yes;
}

bool BookTablePrivate::isUndoable() const
{
    return m_isUndoable;
}

BookTable::BookTable(QWidget *parent) : QWidget(parent)
{
    d_ptr = new BookTablePrivate(this);
    applySettings();

    // initialize empty state
    DeltaDataList dataList;
    emit addCommand(dataList);
}

BookTable::~BookTable()
{
    delete d_ptr;
    d_ptr = nullptr;
}

void setNameToTable(Table* table)
{
    class alphabet
    {
    public:
        QString operator()(int index)
        {
            char baseChar = 'A';
            QString strChar = QString("%1").arg((char)(baseChar + index % 26));
            return strChar;
        }
    };

    // check titles
    for (int i = 0; i < table->size(); ++i)
        if (table->at(i)->name().isEmpty())
            table->at(i)->setName(alphabet()(i));

}

/*!
 * \brief 设置数据表格.
 * \param table 数据表
 */
void BookTable::setTable(Table *table)
{
    if (!table)
        return;

    setNameToTable(table);
    table->blockSignals(true);
    d_ptr->table = table;
    d_ptr->m_complexModel->setHorizontalHeaderLabels(table != nullptr ? table->titles() : QStringList());
    d_ptr->m_complexModel->setTable(table);
    d_ptr->m_dataModel->setTable(table);

    disconnect(table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)),
               this, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
    connect(table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)),
               this, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
    table->blockSignals(false);

    for (int i = 0; i < table->size(); ++i) {
        notifyColumnDataChanged(i, table->at(i).data());
        table->at(i)->setClean(false);
    }
}

/*!
 * \brief 获取数据表格.
 * \return 数据表指针
 */
ComData::Table *BookTable::table() const
{
    return d_ptr->table;
}

/*!
 * \brief 清空底层数据, 且删除列数据结构.
 */
void BookTable::clear()
{
    if (d_ptr->table) {
        d_ptr->table->clear();
        emit getDirty();
    }
}

/*!
 * \brief 清空表格数据, 但不删除列数据结构.
 */
void BookTable::clearContents()
{
    ComData::Table* oldTable =  table()->clone();

    for (int col = 0; col < table()->size(); ++col) {
        QSharedPointer<ComData::ColumnBase> cb = table()->at(col);
        if (cb->size() == 0)
            continue;
        cb->clear();
        cb->clearAllMarked();
        cb->validateCells();
    }

    d_ptr->emitDataChangedSignals();

    DeltaDataList dataList = DeltaMaster::diff(oldTable, table());
    emit addCommand(dataList);
    delete oldTable;

    for (int col = 0; col < table()->size(); ++col) {
        QSharedPointer<ComData::ColumnBase> cb = table()->at(col);
        cb->clear();
    }

    emit d_ptr->dataModel()->layoutChanged();
    emit d_ptr->complexModel()->layoutChanged();
    emit getDirty();
}

/*!
 * \brief 导出数据位Excel文件.
 */
void BookTable::exportAsExcel()
{
    QString excelFile = QFileDialog::getSaveFileName(this, tr("Export as Excel Files - %1").arg(QApplication::applicationDisplayName()), ".", tr("Excel Files(*.xlsx)"));
    if (excelFile.isEmpty())
        return;

    // make a new worksheet
}

/*!
 * \brief 导出数据位HTML表格.
 */
void BookTable::exportAsHtml()
{
    QString htmlFile = QFileDialog::getSaveFileName(this, tr("Export as HTML Files - %1").arg(QApplication::applicationDisplayName()), ".", tr("HTML Files(*.html)"));
    if (htmlFile.isEmpty())
        return;

    if (!(htmlFile.endsWith(".html") || htmlFile.endsWith("htm")))
        htmlFile += ".html";

    ExportThread* worker = new ExportThread(htmlFile, d_ptr->table);
    worker->setFunctor(&Generator<Html>::write);
    QThread* thread = new QThread();
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(run()));
    connect(worker, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    thread->start();
}

/*!
 * \brief 设置是否为自动释放, 自动释放为在类的析构函数中是否删除表格对象.
 * \param release 是否自动释放
 */
void BookTable::setAutoRelease(bool release)
{
    if (d_ptr->isAutoRelease != release)
        d_ptr->isAutoRelease = release;
}

/*!
 * \brief 获取是否自动释放表格对象.
 * \return 是否自动释放
 */
bool BookTable::isAutoRelease() const
{
    return d_ptr->isAutoRelease;
}

/*!
 * \brief 获取横向表头标签.
 * \return 标签
 */
QStringList BookTable::horizontalHeaderLabels() const
{
    return d_ptr->m_complexModel->horizontalHeaderLabels();
}

/*!
 * \brief 获取竖向表头标签.
 * \return 竖向表头标签
 */
QStringList BookTable::verticalHeaderLabels() const
{
    QStringList labels;
    labels += d_ptr->m_complexModel->verticalHeaderLabels();
    labels += d_ptr->m_dataModel->verticalHeaderLabels();
    return labels;
}

QVariant BookTable::headerData(const QModelIndex& index) const
{
    return d_ptr->m_complexModel->data(index);
}

QVariant BookTable::data(const QModelIndex& index) const
{
    return d_ptr->m_dataModel->data(index);
}

QVariant BookTable::headerData(int row, int column) const
{
    return headerData(d_ptr->m_complexModel->index(row, column));
}

QVariant BookTable::data(int row, int column) const
{
    return data(d_ptr->m_dataModel->index(row, column));
}

/*!
 * \brief 返回表的列数.
 */
int BookTable::columnsCount() const
{
    return d_ptr->complexTable->horizontalHeader()->count();
}

/*!
 * \brief 设置当前列, 高亮显示.
 * \param column 列索引
 */
void BookTable::setCurrentColumn(int column)
{
    d_ptr->complexTable->horizontalHeader()->sectionPressed(column);
}

/*!
 * \brief 设置列 \a column 的物理意义为 \a sig.
 * \param column 表中的列索引
 * \param sig 物理意义标识
 */
void BookTable::setColumnSignificance(int column, ColumnBase::Significance sig)
{
    // todo 优化，直接获取一列的标题
    ColumnPropertyData& cpd = d_ptr->columnPropDataMap[column];
    if (sig != cpd.significance()) {
        cpd.setSignificance(sig);
        emit columnPropertyChanged(cpd);
    }

    QSharedPointer<ComData::ColumnBase> colData = d_ptr->table->at(column);
    emit addCommand(DeltaMaster::significanceDiff(column, colData->significance(), sig));
    setColumnSignificance_(column, sig);
    emit getDirty();
}

void BookTable::setColumnsSignificance(const QList<int> &columns, ColumnBase::Significance sig)
{
    QMap<int, QPair<ComData::ColumnBase::Significance, ComData::ColumnBase::Significance> > colSigs;
    foreach (int col, columns)
        colSigs.insert(col, qMakePair(d_ptr->table->at(col)->significance(), sig));
    emit addCommand(DeltaSetSignificances::create(colSigs));
}

QScrollBar* BookTable::horizontalScrollBar() const
{
    return d_ptr->dataTable->horizontalScrollBar();
}

QScrollBar* BookTable::verticalScrollBar() const
{
    return d_ptr->dataTable->verticalScrollBar();
}

/*!
 * \brief resetMenuMangerTable
 *  设置当前的BookTable
 */

void BookTable::setColumnProperty(const ColumnPropertyData& cpd)
{
    if (cpd == d_ptr->columnPropertyData && cpd.isValid()) {
        qDebug() << "same property, quit.";
        return;
    }

    d_ptr->columnPropertyData = cpd;

    // add command
    emit addCommand(DeltaColumnPropChanged::create(d_ptr->columnPropertyData, cpd));
}

QList<QSharedPointer<ColumnBase>> BookTable::selectedColumnData() const
{
    QList<QSharedPointer<ColumnBase>> cols;
    DataModel* dm = qobject_cast<DataModel*>(d_ptr->dataTable->model());
    for (int i = 0; i < d_ptr->dataTable->model()->columnCount(); ++i)
        if (d_ptr->dataTable->selectionModel()->isColumnSelected(i, QModelIndex()))
            cols.append(dm->column(i));

    return cols;
}

/*!
 * \brief 获取选中列的索引列表.
 * \return
 */
QModelIndexList BookTable::selectedColumns() const
{
    return d_ptr->complexTable->selectionModel()->selectedColumns();
}

/*!
 * \brief 返回表中选中列的索引号列表.
 */
QList<int> BookTable::selectedColumnIndex() const
{
    QList<int> index;
    for (int i = 0; i < d_ptr->dataTable->model()->columnCount(); ++i)
        if (d_ptr->dataTable->selectionModel()->isColumnSelected(i, QModelIndex()))
            index.append(i);

    return index;
}

int BookTable::selectedDataRow() const
{
    QModelIndexList indexes = d_ptr->dataTable->selectionModel()->selectedRows();
    if (indexes.isEmpty())
        return -1;
    return indexes.first().row();
}

/*!
 * \brief 获取列名列表.
 * \return 名称列表
 */
QStringList BookTable::columnList() const
{
    QStringList list;
    for (int i = 0; i < d_ptr->m_dataModel->columnCount(); ++i)
        list += d_ptr->m_dataModel->column(i).data()->name();
    return list;
}

/*!
 * \brief 设置是否显示网格.
 * \param visible 可见
 */
void BookTable::setShowGrid(bool visible)
{
    // the source of the wrecked book
    d_ptr->complexTable->setShowGrid(visible);
    d_ptr->dataTable->setShowGrid(visible);
}

/*!
 * \brief 交换列.
 * \param from 源索引
 * \param to 目标索引
 */
void BookTable::swapColumn(int from, int to)
{    
    if (from == to)
        return;

    emit addCommand(DeltaSwapColumn::create(from, to));
}

/*!
 * \brief 移动列.
 * \param from 源索引
 * \param to 目标索引
 */
void BookTable::moveColumn(int from, int to)
{
    if (from == to)
        return;

    emit addCommand(DeltaMoveColumn::create(from, to));
}

/*!
 * \brief 设置行是否可见.
 * \param rowName 行名称
 * \param visible 是否可见
 */
void BookTable::setTableRowVisible(const QString &rowName, bool visible)
{
    emit addCommand(DeltaMaster::rowVisibilityDiff(rowName, d_ptr->isTableRowVisible(rowName), visible));
}

void BookTable::setTableElementVisible(const QString &elem, bool show)
{
    emit addCommand(DeltaMaster::elementVisibilityDiff(elem, !show, show));
    d_ptr->setTableElementVisible(elem, show);
}

/*!
 * \brief 设置列宽.
 * \param width 宽度
 */
void BookTable::setColumnWidth(int width)
{
    d_ptr->columnWidth = width;
    for (int i = 0; i< d_ptr->m_complexModel->columnCount(); i++)
        d_ptr->complexTable->setColumnWidth(i, d_ptr->columnWidth);
}

void BookTable::read(QDataStream &in)
{
    if (d_ptr->table)
        d_ptr->table->clear();

    int columnCount;
    in >> columnCount;

    QStringList hLabels;

    // we need to display the previews, but
    // we also need to prevent emitting 'getDirty' signal
    // so block the gd signals
    blockSignals(true);
    for (int i = 0; i < columnCount; ++i) {
        quint8 type = 0;
        quint32 size = 0;
        in >> type >> size;
        ComData::ColumnBase::ColumnType ct = static_cast<ComData::ColumnBase::ColumnType>(type);
        ComData::ColumnBase* col = ComData::ColumnBase::createColumn(ct, size);
        col->unserialize(in);
        // for displaying data
        col->setClean(false);

        hLabels.append(col->title());
        d_ptr->m_dataModel->addColumn(col);
        emit col->dataChanged();
    }
    blockSignals(false);
    d_ptr->m_complexModel->setHorizontalHeaderLabels(hLabels);
}

void BookTable::write(QDataStream &out) const
{
    out << d_ptr->m_dataModel->columnCount();
    for (int i = 0; i < d_ptr->m_dataModel->columnCount(); ++i)
        d_ptr->m_dataModel->column(i)->serialize(out);
}

bool BookTable::write(QXlsx::Worksheet *sheet) const
{
    return d_ptr->write(sheet);
}

void BookTable::undo()
{
    if (d_ptr->isUndoable())
        d_ptr->undoManager->undo();
}

void BookTable::redo()
{
    if (d_ptr->isUndoable())
        d_ptr->undoManager->redo();
}

void BookTable::setUndoable(bool yes)
{
    d_ptr->setUndoable(yes);
}

bool BookTable::isUndoable() const
{
    return d_ptr->isUndoable();
}

/*!
 * \brief 更新表格的显示, 同步底层数据和UI.
 */
void BookTable::update()
{
    QWidget::update();
    emit d_ptr->m_complexModel->layoutChanged();
    emit d_ptr->m_dataModel->layoutChanged();
}

void BookTable::zoomIn()
{
    qDebug() << __FUNCTION__;
#if 0
    d_ptr->complexTable->zoomIn();
    d_ptr->dataTable->zoomIn();
#endif
}

void BookTable::zoomOut()
{
    qDebug() << __FUNCTION__;
#if 0
    d_ptr->complexTable->zoomIn();
    d_ptr->dataTable->zoomIn();
#endif
}

void BookTable::zoomReset()
{
    qDebug() << __FUNCTION__;
}

/*!
 * \brief 在表中 \a column 列索引处插入类型是 \a ct 的列.
 * \param column 插入列的索引
 * \param ct 列类型
 */
void BookTable::insertColumn(int column, ColumnBase::ColumnType ct)
{
    ColumnBase* cb = ColumnBase::createColumn(ct, 0, d_ptr->table);

    DeltaDataList ddl;
    ddl.append(DeltaInsertColumn::create(column, cb));
    emit addCommand(ddl);

    d_ptr->updateMinSize();
}

void BookTable::appendColumn(ColumnBase::ColumnType ct)
{
    Table* table = d_ptr->table;
    ColumnBase* cb = ColumnBase::createColumn(ct, 0, table);
//    addColumn(cb);

    DeltaDataList ddl;
    ddl.append(DeltaAppendColumn::create(cb));
    emit addCommand(ddl);
    delete cb;

    d_ptr->updateMinSize();
}

/*!
 * \brief 添加列.
 * \param columnData 列数据
 * \param displayCurve 是否显示缩略图
 */
void BookTable::addColumn(ColumnBase *columnData, bool displayCurve)
{
    if (!columnData)
        return;

    Table* table = d_ptr->table;
    QString strTitle = table->autoColumnName();
    columnData->setName(strTitle);
    table->append(QSharedPointer<ColumnBase>(columnData));
    QStringList qsl = d_ptr->m_complexModel->horizontalHeaderLabels();
    qsl.append(strTitle);
    d_ptr->m_complexModel->setHorizontalHeaderLabels(qsl);

    emit d_ptr->m_complexModel->insertColumn(table->size());
    emit d_ptr->m_dataModel->insertColumn(table->size());
    emit d_ptr->m_dataModel->layoutChanged();
    emit d_ptr->m_complexModel->layoutChanged();

    if (displayCurve)
        emit columnData->dataChanged();
    d_ptr->m_complexModel->setHorizontalHeaderLabels(d_ptr->table->titles());
    alignTableColumns();
    d_ptr->updateMinSize();
}

/*!
 * \brief 设置表中单元格数据是否标记.
 * \param mark
 */
void BookTable::setSelectedCellsMarked(bool mark)
{
    QModelIndexList indexList = setSelectedCellsMarked_(mark);
    emit addCommand(DeltaMaster::markCellDiff(indexList, !mark, mark));
}

void BookTable::setColumnsVisible(const QList<int> &cols, bool visible)
{
    emit addCommand(DeltaColumnVisibility::create(cols, !visible, visible));
}

/*!
 * \brief 如果 \a hide 是true, 则隐藏 \a column 列, 否则显示该列.
 * \param column
 * \param hide
 */
bool BookTable::isColumnVisible(int column)
{
    return !d_ptr->complexTable->isColumnHidden(column);
}

/*!
 * \brief 排序类数据.
 * \param column 列索引
 * \param so 升降序
 */
void BookTable::sortColumn(int column, Qt::SortOrder so)
{
    ComData::Table* oldTable = table()->clone();
    d_ptr->table->at(column)->sort(so);

    DeltaDataList ddl = DeltaMaster::diff(oldTable, table());
    if (ddl.isEmpty())
        return;

    emit addCommand(ddl);
}

/*!
 * \brief 排序指定列的数据.
 * \param column 列索引
 * \param so 顺序
 */
void BookTable::sortTable(int column, Qt::SortOrder so)
{
    ComData::Table* oldTable = table()->clone();
    table()->sortByColumn(column, so);

    DeltaDataList dataList = DeltaMaster::diff(oldTable, d_ptr->table);
    emit addCommand(dataList);
    delete oldTable;
    emit getDirty();
}

/*!
 * \brief 用行号填充所选表格.
 */
void BookTable::fillRowNumbers()
{
    Selection srv = d_ptr->dataTable->selectionRangeRegion();
    if (!srv.isValid())
        return;

    ComData::Table* oldTable = table()->clone();
    DataModel* model = d_ptr->dataModel();

    d_ptr->blockTableSignals(true);
    for (int col = srv.startColIndex; col <= srv.endColIndex; col++)
        for (int row = srv.startRowIndex; row <= srv.endRowIndex; row++)
            model->setData(model->index(row, col), row + 1);
    d_ptr->blockTableSignals(false);
    d_ptr->emitDataChangedSignals();

    DeltaDataList ddl = DeltaMaster::diff(oldTable, table());
    delete oldTable;
    if (!ddl.isEmpty())
        emit addCommand(ddl);
    emit model->layoutChanged();
    emit getDirty();
}

inline static double random(int low, int high)
{
    double f = (double)qrand() / RAND_MAX;
    return f * (high - low) + low;
}

/*!
 * \brief 用均匀随机函数填充所选表格.
 */
void BookTable::fillUniformRandom()
{
    Selection srv = d_ptr->dataTable->selectionRangeRegion();
    if (!srv.isValid())
        return;

    ComData::Table* oldTable = table()->clone();
    DataModel* model = d_ptr->dataModel();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    d_ptr->blockTableSignals(true);
    for (int ci = srv.startColIndex; ci <= srv.endColIndex; ci++)
        for (int i = srv.startRowIndex; i <= srv.endRowIndex; i++)
            model->setData(model->index(i, ci), random(0, 1));
    d_ptr->blockTableSignals(false);
    d_ptr->emitDataChangedSignals();

    DeltaDataList ddl = DeltaMaster::diff(oldTable, table());
    delete oldTable;
    if (!ddl.isEmpty())
        emit addCommand(ddl);
    emit model->layoutChanged();
    emit getDirty();
}

/*!
 * \brief 用随机数填充所选表格.
 */
void BookTable::fillRandom()
{
    Selection srv = d_ptr->dataTable->selectionRangeRegion();
    if (!srv.isValid())
        return;

    ComData::Table* oldTable = table()->clone();
    DataModel* model = d_ptr->dataModel();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    d_ptr->blockTableSignals(true);
    for (int col = srv.startColIndex; col <= srv.endColIndex; col++)
        for (int row = srv.startRowIndex; row <= srv.endRowIndex; row++)
            model->setData(model->index(row, col), random(-2, 2));
    d_ptr->blockTableSignals(false);
    d_ptr->emitDataChangedSignals();

    DeltaDataList ddl = DeltaMaster::diff(oldTable, table());
    delete oldTable;
    if (!ddl.isEmpty())
        emit addCommand(ddl);
    emit model->layoutChanged();
    emit getDirty();
}

/*!
 * \brief 追加行.
 */
void BookTable::appendRow()
{
    if (d_ptr->table) {
        DeltaDataList dataList;
        dataList.append(DeltaAppendRow::create(d_ptr->table));
        emit addCommand(dataList);
        emit d_ptr->dataModel()->layoutChanged();
        d_ptr->updateMinSize();
        emit getDirty();
    }
}

/*!
 * \brief 插入行.
 * \param row 行索引
 */
void BookTable::insertRow(int row)
{
    if (d_ptr->table && row >= 0) {
        DeltaDataList dataList;
        dataList.append(DeltaInsertRow::create(d_ptr->table, row));
        emit addCommand(dataList);
        emit d_ptr->dataModel()->layoutChanged();
        d_ptr->updateMinSize();
        emit getDirty();
    }
}


void BookTable::updateTableItemMenu()
{
    QStringList titles = d_ptr->complexModel()->verticalHeaderLabels();
    ComplexTableView *ctv = d_ptr->complexView();

    ActionContainer* ac = ActionManager::actionContainer(ComDataGui::Constants::M_SHOWCOMPLEXROW);
    QList<QAction*> acts = ac->menu()->actions();
    for (QAction *act : acts) {
        // in case of generating redundant undos
        // we block signal emitting for now
        act->blockSignals(true);
        act->setChecked(!ctv->isRowHidden(titles.indexOf(act->text())));
        act->blockSignals(false);
    }
}

void BookTable::updateTableElemMenu()
{
    ActionContainer* ac = ActionManager::actionContainer(ComDataGui::Constants::M_WORKSHEETSHOW);
    for (QAction* act : ac->menu()->actions()) {
        // in case of generating redundant undos
        // we block signal emitting for now
        act->blockSignals(true);
        const QString id = act->data().toString();
        if (id ==  ComDataGui::Constants::WORKSHEETSHOWHEADERLABEL)
            act->setChecked(d_ptr->complexModel()->isVerticalLabelVisible());
        else if (id == ComDataGui::Constants::WORKSHEETSHOWCOLUMNHEADER)
            act->setChecked(d_ptr->complexView()->horizontalHeader()->isVisible());
        else if (id ==  ComDataGui::Constants::WORKSHEETSHOWROWHEADER)
            act->setChecked(d_ptr->isVerticalHeaderVisible());
        else if (id ==  ComDataGui::Constants::WORKSHEETSHOWGRID)
            act->setChecked(d_ptr->complexView()->showGrid());

        act->blockSignals(false);
    }
}

void BookTable::applySettings()
{
    d_ptr->m_complexModel->applySettings();
    d_ptr->m_dataModel->applySetting();
    CommonUtils::SettingsManager* sm = CommonUtils::SettingsManager::instance();
    QSettings *setting = sm->generalSettings();

    // column width
    d_ptr->columnWidth = setting->value(QStringLiteral("Table/ColumnWidth"), 100).toInt();

    for (int i = 0; i< d_ptr->m_complexModel->columnCount(); i++)
        d_ptr->complexTable->setColumnWidth(i, d_ptr->columnWidth);

    bool isAlterRowColor = setting->value(QStringLiteral("Table/AlterNate"), false).toBool();
    d_ptr->dataTable->setAlternatingRowColors(isAlterRowColor);

    emit d_ptr->dataTable->model()->layoutChanged();
    emit d_ptr->complexTable->model()->layoutChanged();
}

/*!
 * \brief 删除列.
 * \param column 列索引
 */
void BookTable::removeColumn(int column)
{
    // add remove single column delta
    removeColumn_(column);
    emit getDirty();
}

/*!
 * \brief 删除一个或多个列.
 * \param columns 列索引列表
 */
void BookTable::removeColumns(const QList<int> &columns)
{
    QMap<int, ComData::ColumnBase*> columnDataMap;
    foreach (int col, columns) {
        ComData::ColumnBase* colData = d_ptr->table->at(col)->clone();
        columnDataMap.insert(col, colData);
    }

    DeltaDataList ddl;
    ddl.append(DeltaDeleteColumn::create(columnDataMap));
    emit addCommand(ddl);
    emit getDirty();
}

/*!
 * \brief 删除列, 内部用函数.
 * \param index 列索引
 */
void BookTable::removeColumn_(int column)
{
    if (!d_ptr->table->removeAt(column))
        qDebug() << "failed to remove column: " << column;

    emit d_ptr->complexModel()->layoutChanged();
    emit d_ptr->dataModel()->layoutChanged();
    d_ptr->m_complexTableHeight = d_ptr->complexTableHeight();
    d_ptr->updateGeo(true);
    d_ptr->updateMinSize();
}

ComData::ColumnBase* BookTable::insertColumn_(int column, ColumnBase::ColumnType ct)
{
    Table* table = d_ptr->table;
    ColumnBase* cb = ColumnBase::createColumn(ct, 0, table);
    QString strTitle = table->autoColumnName();
    cb->setName(strTitle);
    cb->setSignificance(ColumnBase::Y);
    table->insert(column, QSharedPointer<ColumnBase>(cb));

    QStringList qsl = d_ptr->m_complexModel->horizontalHeaderLabels();
    qsl.insert(column, strTitle);
    d_ptr->m_complexModel->setHorizontalHeaderLabels(qsl);
//    d_ptr->m_complexModel->insertColumns(column, 1);
    d_ptr->complexTable->removeThumbnails();

    for (int i = 0; i < d_ptr->m_dataModel->columnCount(); ++i) {
        ColumnBase* cb = d_ptr->m_dataModel->column(i).data();
        emit cb->dataChanged();
    }

    emit d_ptr->m_complexModel->layoutChanged();
    emit d_ptr->m_dataModel->layoutChanged();
    emit d_ptr->m_complexModel->layoutChanged();
    d_ptr->m_complexModel->setHorizontalHeaderLabels(d_ptr->table->titles());
    d_ptr->updateMinSize();
    return cb;
}

void BookTable::setColumnProperty_(const ColumnPropertyData& cpd)
{
    // MARK, here miss the UNDO/REDO feature
    // how to tell if there're multiple columns selected
    int selectedColumns = d_ptr->complexTable->selectionModel()->selectedColumns().size();
    if (selectedColumns > 1)
        return;

    // update properties to complex & data table/model
    d_ptr->complexTable->setLongName(cpd.column(), cpd.longName());
    d_ptr->complexTable->setUnit(cpd.column(), cpd.unit());

    ComData::ColumnBase* col = d_ptr->m_dataModel->column(cpd.column()).data();
    col->setLongName(cpd.longName());
    col->setUnit(cpd.unit());
    col->setName(cpd.shortName());
    col->setSignificance(cpd.significance());
    d_ptr->setCurrentColumnSignificance(cpd.significance());

    d_ptr->m_dataModel->setColumnPrecision(cpd.column(), cpd.precision());
    d_ptr->m_dataModel->setColumnFormat(cpd.column(), cpd.useDecimal() ? 'f' : 'd');
    d_ptr->m_dataModel->setColumnType(cpd.column(), cpd.columnType());

    // why the hell add thumbnail update mechanism ?
//    d_ptr->complexView()->removeThumbnails();
//    for (int i = 0; i < d_ptr->table->size(); ++i)
//        notifyColumnDataChanged(i, d_ptr->table->at(i).data());

    // modify the data structure
    d_ptr->columnPropDataMap[cpd.column()] = cpd;
}

void BookTable::swapColumn_(int from, int to)
{
    d_ptr->table->swap(from, to);
    d_ptr->updateHorizontalLabels();
    d_ptr->complexTable->swapColumn(from, to);
    d_ptr->complexTable->onColumnDataChanged(from, d_ptr->table->at(from).data());
    d_ptr->complexTable->onColumnDataChanged(to, d_ptr->table->at(to).data());

    emit d_ptr->m_dataModel->layoutChanged();
    emit d_ptr->m_complexModel->layoutChanged();
}

void BookTable::moveColumn_(int from, int to)
{
    d_ptr->table->moveTo(from, to);
    d_ptr->updateHorizontalLabels();
    d_ptr->complexTable->moveColumn(from, to);
    d_ptr->complexTable->onColumnDataChanged(from, d_ptr->table->at(from).data());
    d_ptr->complexTable->onColumnDataChanged(to, d_ptr->table->at(to).data());

    emit d_ptr->m_dataModel->layoutChanged();
    emit d_ptr->m_complexModel->layoutChanged();
}

void BookTable::updateMinSize()
{
    d_ptr->updateMinSize();
}

/*!
 * \brief 双击标头排序
 * \param index
 */
void BookTable::onHorizontalHeaderDoubleClicked(int index)
{
    // if index is negative it will crash
    if (index < 0)
        return;

    d_ptr->complexTable->horizontalHeader()->setSortIndicator(index, d_ptr->horizontalSortOrder);

    ComData::Table* oldTable = table()->clone();

    d_ptr->m_dataModel->sortColumn(index, d_ptr->horizontalSortOrder);

    DeltaDataList dataList = DeltaMaster::diff(oldTable, table());
    emit addCommand(dataList);
    delete oldTable;

    if (d_ptr->horizontalSortOrder == Qt::AscendingOrder)
        d_ptr->horizontalSortOrder = Qt::DescendingOrder;
    else
        d_ptr->horizontalSortOrder = Qt::AscendingOrder;
}

void BookTable::setAlternatingRowColors(bool yes)
{
    d_ptr->dataTable->setAlternatingRowColors(yes);
}

void BookTable::setRowCount(int count)
{
    d_ptr->m_dataModel->setRowCount(count);
}

void BookTable::setFont(const QFont& font)
{
    d_ptr->m_dataModel->setFont(font);
}

void BookTable::setFontColor(const QColor& clr)
{
    d_ptr->m_dataModel->setFontColor(clr);
}

void BookTable::setDataBackgroundColor(const QColor& clr)
{
    d_ptr->m_dataModel->setBackgroundColor(clr);
}

void BookTable::setHeaderBackgroundColor(const QColor &clr)
{
    d_ptr->m_complexModel->setBackgroundColor(clr);
}

void BookTable::setDisplayFormat(char format)
{
    d_ptr->m_dataModel->setDisplayFormat(format);
}

void BookTable::setPrecision(int precision)
{
    d_ptr->m_dataModel->setPrecision(precision);
}

/*!
 * \reimp
 */
QSize BookTable::sizeHint() const
{
    return d_ptr->size();
}

/*!
 * \reimp
 */
QSize BookTable::minimuSizeHint() const
{
    return d_ptr->size();
}

/*!
 * \reimp
 */
void BookTable::showEvent(QShowEvent *)
{
    if (d_ptr->isFirstShown) {
        d_ptr->updateGeo();
        d_ptr->m_complexTableHeight = d_ptr->complexTable->height();
        d_ptr->adjustHeaderWidth();
        d_ptr->isFirstShown = false;
    }

    alignTableColumns();
}

/*!
 * \reimp
 */
void BookTable::resizeEvent(QResizeEvent *)
{
    d_ptr->updateGeo(true);
    d_ptr->adjustHeaderWidth();
    d_ptr->updateMinSize();
}

void BookTable::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    d_ptr->dataTable->setColumnWidth(logicalIndex, newSize);
}

void BookTable::onSectionPressed(int logicalindex)
{
    return;
    qDebug() << "onSectionPressed: " << logicalindex;
    d_ptr->dataTable->selectionModel()->clearSelection();
    d_ptr->dataTable->selectColumn(logicalindex);

    DataModel* dataModel = dynamic_cast<DataModel*>(d_ptr->dataTable->model());

    if (logicalindex != -1 && logicalindex < dataModel->columnCount()) {
        QSharedPointer<ColumnBase> meta = dataModel->column(logicalindex);
        qDebug() << "meta type: " << meta->type();
        QTime time;
        time.start();
        //        for (int i = 0; i < col.count(); ++i)
        //            double d = col.at(i);
        //        //            qDebug() << col.at(i).toDouble();
        //        //        qDebug() << col;
        //        qDebug() << time.elapsed() << "ms";
        //        qDebug() << col;
    }
}

void BookTable::onDataSectionPressed(int logicalindex)
{
    Q_UNUSED(logicalindex);
    d_ptr->complexTable->selectionModel()->clearSelection();
}

void BookTable::onComplexSectionPressed(int logicalindex)
{
    Q_UNUSED(logicalindex);
    d_ptr->dataTable->selectionModel()->clearSelection();
}

void BookTable::onDataHeaderGeoChanged()
{
    // no impl
}

void BookTable::onComplexHeaderGeoChanged()
{
    // no impl
}

void BookTable::onDataCellClicked(const QModelIndex& index)
{
    d_ptr->complexTable->selectionModel()->clearSelection();
    if (!d_ptr->columnPropDataMap.contains(index.column()))
        onHorizontalHeaderClicked(index.column());

    emit setCurrentColumnProperty(d_ptr->columnPropDataMap.value(index.column()));
}

void BookTable::onComplexCellClicked(const QModelIndex& index)
{
    d_ptr->dataTable->selectionModel()->clearSelection();
    if (!d_ptr->columnPropDataMap.contains(index.column()))
        onHorizontalHeaderClicked(index.column());
    emit setCurrentColumnProperty(d_ptr->columnPropDataMap.value(index.column()));
}

void BookTable::onComplexDataInserted(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
}

void BookTable::onComplexDataRemoved(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
}

void BookTable::onComplexHeaderSectionResized(int logicalindex, int oldsize, int newsize)
{
    Q_UNUSED(logicalindex);
    Q_UNUSED(oldsize);
    Q_UNUSED(newsize);
    d_ptr->m_complexTableHeight = d_ptr->m_complexTableHeight - oldsize + newsize;
    d_ptr->updateGeo(true);
}

void BookTable::onComplexIndexDoubleClicked(const QModelIndex &index)
{
    d_ptr->complexTable->edit(index);
}

void BookTable::onMoveOutOfDataTable(int column)
{
    qDebug() << "move out of data table";
    if (d_ptr->complexTable->model()->rowCount() == 0)
        return;

    const int lastRow = d_ptr->complexModel()->rowCount() - 1;
    d_ptr->updateIndex(d_ptr->dataTable, d_ptr->complexTable, qobject_cast<TableModelBase*>(d_ptr->complexModel()), lastRow, column, lastRow);
}

void BookTable::onMoveOutOfComplexTable(int column)
{
    qDebug() << "move out of complex table";
    if (d_ptr->dataTable->model()->rowCount() == 0)
        return;

    int visibleRow = d_ptr->dataTable->verticalScrollBar()->value();
    d_ptr->updateIndex(d_ptr->complexTable, d_ptr->dataTable, qobject_cast<TableModelBase*>(d_ptr->dataModel()), visibleRow, column, 0);
}

void BookTable::onVBarValueChanged_DataTable(int value)
{
    Q_UNUSED(value);
    d_ptr->updateGeo(true, false);
}

void BookTable::onDataTableClearSelection()
{
    qDebug() << "clean complex table selection";
    d_ptr->complexTable->selectionModel()->clearSelection();
}

void BookTable::onComplexTableClearSelection()
{
    qDebug() << "clean data table selection";
    d_ptr->dataTable->selectionModel()->clearSelection();
}

/*!
 * \brief 当非数据表被点击的时候清除数据表的选中区域,
 * 从而是两个表格的行为看上去像一个表格.
 */
void BookTable::onComplexTablePressed()
{
    d_ptr->dataTable->selectionModel()->clear();
}

/*!
 * \brief 当数据表被点击的时候清除非数据表的选中区域,
 * 从而是两个表格的行为看上去像一个表格.
 */
void BookTable::onDataTablePressed()
{
    d_ptr->complexTable->selectionModel()->clear();
}

void BookTable::onSetDataTableSelectAll()
{
    d_ptr->dataTable->selectAll();
    d_ptr->complexTable->selectAll();
}

void BookTable::onDataChanged(const QModelIndex &index, const QString &section, const QVariant &val)
{
    int col = index.column();
    QSharedPointer<ColumnBase> columnBase = d_ptr->m_dataModel->column(col);

    ColumnPropertyData cpd = d_ptr->columnPropDataMap.value(index.column());
    if (section.compare(ComDataGui::Constants::TABLEROWCOMMENT) == 0)
        columnBase.data()->setComment(val.toString());
    else if (section.compare(ComDataGui::Constants::TABLEROWLONGNAME) == 0) {
        columnBase.data()->setLongName(val.toString());
        cpd.setLongName(val.toString());
    } else if (section.compare(ComDataGui::Constants::TABLEROWUNIT) == 0) {
        columnBase.data()->setUnit(val.toString());
        cpd.setUnit(val.toString());
    }
}

void BookTable::onEvaluate(const QModelIndex &index, const QString &expression)
{
    if (expression.trimmed().isEmpty())
        return;

    ComData::ColumnBase* columnData = d_ptr->m_dataModel->column(index.column()).data();
    ComData::Table* oldTable = table()->clone();

    QString oldExp = columnData->expression();
    columnData->setExpression(expression);
    if (columnData->size() < d_ptr->m_dataModel->rowCount())
        columnData->resize(d_ptr->m_dataModel->rowCount());

    QMap<QString, ComData::ColumnBase*> name2ColumnMap;
    for (int i = 0; i < d_ptr->m_dataModel->columnCount(); ++i) {
        ComData::ColumnBase* col = d_ptr->m_dataModel->column(i).data();
        name2ColumnMap.insert(col->name(), col);
    }

    QScriptEngine myEngine;
    for (int i = 0; i < columnData->size(); ++i) {
        QString exp(expression.trimmed());

        // replace column expression with cell value
        QMap<QString, ComData::ColumnBase*>::iterator iter = name2ColumnMap.begin();
        while (iter != name2ColumnMap.end()) {
            if (exp.contains(iter.key()) && iter.value()->size() > i)
                exp.replace(iter.key(), iter.value()->cell(i).toString());
            ++iter;
        }

        // re-evaluate
        QScriptValue sv = myEngine.evaluate(exp);
        double value = sv.toNumber();
        columnData->setCell(i, qIsNaN(value) ? QVariant() : value);
    }

    DeltaDataList dataList = DeltaMaster::diff(oldTable, table());

    // we don't want mem-leak
    delete oldTable;

    DeltaDataList ddl;
    ddl.append(DeltaCustomFunction::create(columnData, table(), dataList,
                                           oldExp, expression));
    emit addCommand(ddl);

    // update cells' content
    for (int i = 0; i < d_ptr->m_dataModel->rowCount(); ++i)
        d_ptr->dataTable->update(d_ptr->m_dataModel->index(i, index.column()));
}

static void filterColumnData(ColumnBase* col, const BaseComparator<double>* bc, const FilterData& fd)
{
    for (int i = 0; i < col->size(); ++i) {
        if (bc->compare(col->cell(i).toDouble(), fd.threshhold1))
            col->validateCell(i);
        else
            col->setInvalidCell(i);
    }
}

static void filterColumnDataEx(ColumnBase* col, const LogicComparator<double>* logic,
                               const BaseComparator<double>* bc1, const BaseComparator<double>* bc2,
                               const FilterData& fd)
{
    for (int i = 0; i < col->size(); ++i) {
        if (logic->compare(bc1->compare(col->cell(i).toDouble(), fd.threshhold1),
                           bc2->compare(col->cell(i).toDouble(), fd.threshhold2)))
            col->validateCell(i);
        else
            col->setInvalidCell(i);
    }
}

/*!
 * \brief 创建比较器.
 * \param bc 比较器智能指针
 * \param type 比较类型
 */
static void createComparator(QScopedPointer<BaseComparator<double> >& bc, int type)
{
    switch (type) {
    case FT_Less:
        bc.reset(new Less<double>());
        break;

    case FT_LessEqual:
        bc.reset(new LessEqual<double>());
        break;

    case FT_Larger:
        bc.reset(new Larger<double>());
        break;

    case FT_LargerEqual:
        bc.reset(new LargerEqual<double>());
        break;

    case FT_Equal:
        bc.reset(new Equal<double>());
        break;

    case FT_NotEqual:
        bc.reset(new NotEqual<double>());
        break;
    }
}

void BookTable::onFilterOut(const QModelIndex& index, const FilterData& fd)
{
    ColumnBase* colData= d_ptr->m_dataModel->column(index.column()).data();
    if (!colData)
        return;

    ComData::Table* oldTable = table()->clone();
    FilterData oldFitler = colData->extra(0).value<FilterData>();

    DeltaDataList dataList = DeltaMaster::diff(oldTable, table());
    DeltaDataList ddl;
    ddl.append(DeltaFilter::create(index, oldFitler, fd, table(), dataList));
    emit addCommand(ddl);
    delete oldTable;
}

/*!
 * \brief 对数据列按数据的有效性进行排序, 有效的排前面, 无效的排后面.
 * \param colData 数据列对象
 */
static inline void sortByValidity(ColumnBase* colData)
{
    ColumnBase* tmpClone = colData->clone();
    int colDataIndex = 0;
    for (int i = 0; i < tmpClone->size(); ++i)
        if (tmpClone->isValid(i))
            colData->setCell(colDataIndex++, tmpClone->cell(i));

    for (int i = 0; i < tmpClone->size(); ++i)
        if (!tmpClone->isValid(i))
            colData->setCell(colDataIndex++, tmpClone->cell(i));

    delete tmpClone;
}

void BookTable::onFilterOut_(const QModelIndex &index, const FilterData &fd)
{
    ColumnBase* colData= d_ptr->m_dataModel->column(index.column()).data();
    if (!colData)
        return;

    FilterData oldFilter = colData->extra(0).value<FilterData>();
    oldFilter.print();

    QVariant v;
    colData->setExtra(0, v.fromValue(fd));

    QScopedPointer<BaseComparator<double> > bc1;
    QScopedPointer<BaseComparator<double> > bc2;

    if (fd.type1 >= 0)
        createComparator(bc1, fd.type1);

    if (fd.type2 >= 0) {
        createComparator(bc2, fd.type2);
        QScopedPointer<LogicComparator<double> > logic;
        if (fd.op == FilterData::And) {
            logic.reset(new AndComparator<double>);
            filterColumnDataEx(colData, logic.data(), bc1.data(), bc2.data(), fd);
        } else {
            logic.reset(new OrComparator<double>);
            filterColumnDataEx(colData, logic.data(), bc1.data(), bc2.data(), fd);
        }
    } else if (!bc1.isNull())
        filterColumnData(colData, bc1.data(), fd);

    if (fd.toString().trimmed().isEmpty()) {
        for (int i = 0; i < colData->size(); ++i)
            colData->validateCell(i);
    }

    sortByValidity(colData);

    // update cells' content
    for (int i = 0; i < d_ptr->m_dataModel->rowCount(); ++i)
        d_ptr->dataTable->update(d_ptr->m_dataModel->index(i, index.column()));
}

void BookTable::onResetColumn(const QModelIndex &index)
{
    ColumnBase* columnData = d_ptr->m_dataModel->column(index.column()).data();
    for (int i = 0; i < columnData->size(); ++i)
            columnData->validateCell(i);
}

void BookTable::onHorizontalHeaderClicked(int index)
{
    // inherit some properties from parent, and create column
    // specific properties
    if (d_ptr->columnPropDataMap.contains(index)) {
        emit setCurrentColumnProperty(d_ptr->columnPropDataMap.value(index));
    } else {
        ColumnPropertyData cpd;
        ComData::ColumnBase* col = d_ptr->m_dataModel->column(index).data();
        cpd.setColumn(index);
        cpd.setPrecision(d_ptr->m_dataModel->precision());
        cpd.setLongName(col->longName());
        cpd.setUnit(col->unit());
        cpd.setShortName(col->name());
        cpd.setSignificance(col->significance());
        cpd.setColumnType(col->type());
        emit setCurrentColumnProperty(cpd);
    }
}

void BookTable::onDataCommitted(int type, const QModelIndex &index, const QVariant &oldValue,
                                const QVariant &newValue, const QVariant& extra)
{
    if (oldValue == newValue)
        return;

    if (type == TableViewBase::Data) {
        DeltaDataList ddl = DeltaMaster::dataCellDiff(table(), index.row(), index.column(), oldValue, newValue);
        emit addCommand(ddl);
    } else {
        if (index.row() == d_ptr->complexModel()->verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)) ||
            index.row() == d_ptr->complexModel()->verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER)))
            return;

        if (index.row() == d_ptr->complexModel()->verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME)))
            d_ptr->columnPropDataMap[index.column()].setLongName(newValue.toString());
        else if (index.row() == d_ptr->complexModel()->verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT)))
            d_ptr->columnPropDataMap[index.column()].setUnit(newValue.toString());

        DeltaDataList ddl = DeltaMaster::complexCellDiff(table(), index.row(), index.column(), oldValue, newValue, extra.toString());
        emit addCommand(ddl);

        emit getDirty();
    }
}

void BookTable::onColumnDataChanged(int column, const ColumnBase *columnData)
{
    Q_UNUSED(column)
    Q_UNUSED(columnData)
    emit getDirty();
}

void BookTable::onRowVisible(const QString &rowName, bool visible)
{
    d_ptr->setTableRowVisible(rowName, visible);
}

void BookTable::setTableRowVisible_(const QString &rowName, bool visible)
{
    d_ptr->setTableRowVisible(rowName, visible);
}

void BookTable::setTableElementVisible_(const QString& elem, bool show)
{
    d_ptr->setTableElementVisible(elem, show);
}

/*!
 * \brief 设置列的意义.
 * \param column 列索引
 * \param sig 意义
 */
void BookTable::setColumnSignificance_(int column, ComData::ColumnBase::Significance sig)
{
    QStringList titles = d_ptr->complexModel()->horizontalHeaderLabels();
    QString strTitle = titles.at(column);
    strTitle = strTitle.left(strTitle.indexOf("("));
    strTitle += sig2str(sig);
    titles.replace(column, strTitle);
    d_ptr->complexModel()->setHorizontalHeaderLabels(titles);
    d_ptr->dataModel()->setSignificance(column, sig);
}

/*!
 * \brief 设置列是否可见.
 * \param column 列索引
 * \param visible 是否可见
 */
void BookTable::setColumnVisible_(int column, bool visible)
{
    d_ptr->complexView()->setColumnHidden(column, !visible);
    d_ptr->dataView()->setColumnHidden(column, !visible);
}

/*!
 * \brief 标记选中单元格.
 * \param mark 是否标记
 * \return 选中单元格索引列表
 */
QModelIndexList BookTable::setSelectedCellsMarked_(bool mark)
{
    DataTableView *dtv = d_ptr->dataView();

    Selection srvMark = dtv->selectionRangeRegion();
    QModelIndexList indexList;
    if (!srvMark.isValid())
        return indexList;

    DataModel* model = d_ptr->dataModel();

    for (int col = srvMark.startColIndex; col <= srvMark.endColIndex; ++col) {
        for (int row = srvMark.startRowIndex; row <= srvMark.endRowIndex; ++row) {
            QModelIndex index = model->index(row, col);
            indexList.append(index);
            model->setMarkedCell(index, mark);
        }
    }

    emit model->layoutChanged();
    return indexList;
}

void BookTable::setCellsMarked(const QModelIndexList &indexList, bool mark)
{
    DataModel* model = d_ptr->dataModel();
    foreach (const QModelIndex& index, indexList)
        model->setMarkedCell(index, mark);

    emit model->layoutChanged();
}

/*!
 * \reimp
 */
void BookTable::contextMenuEvent(QContextMenuEvent *e)
{
    if (!d_ptr->contextMenu)
        d_ptr->contextMenu = new QMenu(this);
    d_ptr->contextMenu->clear();

    if (!selectedColumnIndex().isEmpty()) { // 只要选择了列，则添加Plot与Fill菜单
        ActionContainer* ac = ActionManager::actionContainer(ComDataGui::Constants::M_PLOT);
        d_ptr->contextMenu->addMenu(ac->menu());
        d_ptr->contextMenu->addSeparator();
    }

    int width = 0;
    QHeaderView* hv = d_ptr->complexTable->horizontalHeader();
    for (int i = 0; i < hv->count(); i++)
        width += hv->sectionSize(i);

    // out of bookTable menu
    QHeaderView* verticalhv = d_ptr->complexTable->verticalHeader();
    int bookWidth = verticalhv->isVisible() ? e->pos().x() - verticalhv->width()
                                            : e->pos().x();
    if (bookWidth > width) {
        // 表格数据区域外
        GuiUtils::Command * cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::CLEARTABLE);
        d_ptr->contextMenu->addAction(cmd->action());
        ActionContainer* appendColumnMenu = GuiUtils::ActionManager::actionContainer(ComDataGui::Constants::M_ADDCOLUMN);
        d_ptr->contextMenu->addMenu(appendColumnMenu->menu());
    } else if (e->pos().y() < d_ptr->m_complexTableHeight) {
        // isInComplexArea
        if(e->pos().y() < hv->height() && e->pos().x() > hv->pos().x()) { //点击的是Horizontal Header
            //m_bookMenuManager->setComplexHorizontalHeaderMenu(m_contextMenu);
            // todo 添加cut等
            d_ptr->contextMenu->addSeparator();

            ActionContainer* ac = ActionManager::actionContainer(ComDataGui::Constants::M_COLUMNHEADERSETTING);
            d_ptr->contextMenu->addMenu(ac->menu());

            d_ptr->contextMenu->addSeparator();

            ac = ActionManager::actionContainer(ComDataGui::Constants::M_SHOWCOMPLEXROW);
            d_ptr->contextMenu->addMenu(ac->menu());
            ac = ActionManager::actionContainer(ComDataGui::Constants::M_WORKSHEETSHOW);
            d_ptr->contextMenu->addMenu(ac->menu());

            d_ptr->contextMenu->addSeparator();

            ac = ActionManager::actionContainer(ComDataGui::Constants::M_ADDCOLUMN);
            d_ptr->contextMenu->addMenu(ac->menu());
            ac = ActionManager::actionContainer(ComDataGui::Constants::M_INSERTCOLUMN);
            d_ptr->contextMenu->addMenu(ac->menu());
            GuiUtils::Command * cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::DELETECOLUMN);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::CLEARTABLE);
            d_ptr->contextMenu->addAction(cmd->action());

            d_ptr->contextMenu->addSeparator();

            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::MARK);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::UNMARK);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::SHOWCOLUMN);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::HIDECOLUMN);
            d_ptr->contextMenu->addAction(cmd->action());

            d_ptr->contextMenu->addSeparator();

            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::COLUMNSORTASC);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::COLUMNSORTDESC);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::BOOKTABLESORTASC);
            d_ptr->contextMenu->addAction(cmd->action());
            cmd = GuiUtils::ActionManager::command(ComDataGui::Constants::BOOKTABLESORTDESC);
            d_ptr->contextMenu->addAction(cmd->action());

            if (d_ptr->dataTable->selectionModel()->selectedIndexes().count() > 1) {
                d_ptr->contextMenu->addSeparator();
                ac = ActionManager::actionContainer(ComDataGui::Constants::M_COLUMN_FILLNUMWITH);
                d_ptr->contextMenu->addMenu(ac->menu());
            }
        } else if (e->pos().x() < verticalhv->width() &&
                   e->y() < (verticalhv->height() + hv->height())) {
            // impl later
        }
    } else { // Data Table 区域
        // no data area menu
        // impl later
    }

    if (selectedColumnIndex().isEmpty() &&
            d_ptr->dataTable->selectionModel()->selectedIndexes().count() > 1) {
        d_ptr->contextMenu->addSeparator();
        ActionContainer* ac = ActionManager::actionContainer(ComDataGui::Constants::M_COLUMN_FILLNUMWITH);
        d_ptr->contextMenu->addMenu(ac->menu());

        GuiUtils::Command * appendRowCmd = GuiUtils::ActionManager::command(ComDataGui::Constants::APPENDROW);
        d_ptr->contextMenu->addAction(appendRowCmd->action());

        GuiUtils::Command * insertRowCmd = GuiUtils::ActionManager::command(ComDataGui::Constants::INSERTROW);
        d_ptr->contextMenu->addAction(insertRowCmd->action());
    }

    if (d_ptr->contextMenu->isEmpty())
        return;

    d_ptr->contextMenu->exec(e->globalPos());
}

/*!
 * \reimp
 */
void BookTable::wheelEvent(QWheelEvent *e)
{
    if (d_ptr->isCtrlPressed) {
        if (e->delta() > 0)
            zoomIn();
        else
            zoomOut();
    }
}

/*!
 * \reimp
 */
void BookTable::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control)
        d_ptr->isCtrlPressed = true;
}

/*!
 * \reimp
 */
void BookTable::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control)
        d_ptr->isCtrlPressed = false;
}

/*!
 * \brief 对齐上下两个表的列宽度, 使之看起来是一个表.
 */
void BookTable::alignTableColumns()
{
    for (int i = 0; i < d_ptr->m_dataModel->columnCount(); ++i)
        d_ptr->dataTable->setColumnWidth(i, d_ptr->complexTable->columnWidth(i));
}

/*!
 * \brief 设置上表单元格数据.
 * \param row 行号
 * \param column 列号
 * \param data 数据
 */
void BookTable::setComplexData(int row, int column, const QVariant &data)
{
    QModelIndex index = d_ptr->m_complexModel->createIndex(row, column);
    d_ptr->complexModel()->setData(index, data);
    d_ptr->complexTable->update(index);
}

/*!
 * \brief 通知列数据发生变化, 更新缩略图.
 * \param column 列索引
 * \param columnData 列数据对象
 */
void BookTable::notifyColumnDataChanged(int column, const ColumnBase *columnData)
{
    d_ptr->complexTable->onColumnDataChanged(column, columnData);
}

} // namespace ComDataGui

