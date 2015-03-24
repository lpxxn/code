#ifndef BOOKTABLE_H
#define BOOKTABLE_H

#include "comdatagui_global.h"
#include "columnpropertydata.h"
#include "detail/delta.h"

#include <comdata/column.h>
#include <QAbstractItemModel>
#include <QWidget>

class QAbstractItemModel;
class QScrollBar;
class QSplitter;
class QTableView;
class QMenu;
class QAbstractButton;
class QHeaderView;

namespace ComData
{
    class Table;
}

namespace QXlsx
{
    class Worksheet;
}

namespace ComDataGui
{

class ComplexTableView;
class DataTableView;
class TableViewBase;
class TableModelBase;
class DataModel;
class ComplexModel;
class FilterData;
class BookTablePrivate;
/*!
 * \class 工作表类
 */
class COMDATAGUI_EXPORT BookTable : public QWidget
{
    Q_OBJECT
public:    
    explicit BookTable(QWidget *parent = 0);
    ~BookTable();

    void setTable(ComData::Table* table);
    ComData::Table* table() const;

    void clear();
    void clearContents();

    void exportAsExcel();
    void exportAsHtml();

    void setAutoRelease(bool release);
    bool isAutoRelease() const;

    QStringList horizontalHeaderLabels() const;
    QStringList verticalHeaderLabels() const;

    QVariant headerData(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index) const;

    QVariant headerData(int row, int column) const;
    QVariant data(int row, int column) const;

    void removeColumn(int column);
    void removeColumns(const QList<int>& columns);
    int columnsCount() const;
    void setCurrentColumn(int column);

    void setColumnSignificance(int column, ComData::ColumnBase::Significance sig);
    void setColumnsSignificance(const QList<int>& columns, ComData::ColumnBase::Significance sig);

    void insertColumn(int column, ComData::ColumnBase::ColumnType ct);
    void appendColumn(ComData::ColumnBase::ColumnType ct);

    void setSelectedCellsMarked(bool mark);

    void setColumnsVisible(const QList<int>& cols, bool visible);
    bool isColumnVisible(int column);

    void sortColumn(int column, Qt::SortOrder so);
    void sortTable(int column, Qt::SortOrder so);

    void fillRowNumbers();
    void fillUniformRandom();
    void fillRandom();

    void appendRow();
    void insertRow(int row);

    void updateTableItemMenu();
    void updateTableElemMenu();

    QScrollBar* horizontalScrollBar() const;
    QScrollBar* verticalScrollBar() const;

    void setColumnProperty(const ColumnPropertyData& cpd);

    QList<QSharedPointer<ComData::ColumnBase> > selectedColumnData() const;
    QModelIndexList selectedColumns() const;
    QList<int> selectedColumnIndex() const;
    QStringList columnList() const;
    int selectedDataRow() const;

    void setShowGrid(bool visible);

    void swapColumn(int from, int to);
    void moveColumn(int from, int to);

    void read(QDataStream &in);
    void write(QDataStream &out) const;
    bool write(QXlsx::Worksheet* sheet) const;

    void undo();
    void redo();

    void setUndoable(bool yes);
    bool isUndoable() const;

    void update();

    void zoomIn();
    void zoomOut();
    void zoomReset();

signals:
    void setCurrentColumnProperty(const ComDataGui::ColumnPropertyData& cpd);
    void columnPropertyChanged(const ComDataGui::ColumnPropertyData& cpd);
    void getDirty();

    void addCommand(const DeltaDataList& ddl);
    void addCommand(DeltaBase* delta);

public slots:    
    void addColumn(ComData::ColumnBase* columnData, bool displayCurve = true);
    void setTableRowVisible(const QString &rowName, bool visible);
    void setTableElementVisible(const QString& elem, bool show);

    // MARK: the following apis should add UNDO/REDO function
    void setColumnWidth(int width);
    void setAlternatingRowColors(bool yes);
    void setRowCount(int count);
    void setFont(const QFont& font);
    void setFontColor(const QColor& clr);
    void setDataBackgroundColor(const QColor& clr);
    void setHeaderBackgroundColor(const QColor& clr);
    void setDisplayFormat(char format);
    void setPrecision(int precision);

    void applySettings();
    void alignTableColumns();

    void setComplexData(int row, int column, const QVariant& data);
    void notifyColumnDataChanged(int column, const ComData::ColumnBase* columnData);

protected:
    virtual QSize sizeHint() const;
    virtual QSize minimuSizeHint() const;
    virtual void showEvent(QShowEvent* );
    virtual void resizeEvent(QResizeEvent *);
    virtual void contextMenuEvent(QContextMenuEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

private slots:
    void onHorizontalHeaderDoubleClicked(int index);
    void updateSectionWidth(int logicalIndex, int oldSize, int newSize);

    void onSectionPressed(int logicalindex);
    void onDataSectionPressed(int logicalindex);
    void onComplexSectionPressed(int logicalindex);

    void onDataHeaderGeoChanged();
    void onComplexHeaderGeoChanged();

    void onDataCellClicked(const QModelIndex& index);
    void onComplexCellClicked(const QModelIndex& index);

    void onComplexDataInserted(const QModelIndex& parent, int start, int end);
    void onComplexDataRemoved(const QModelIndex& parent, int start, int end);
    void onComplexHeaderSectionResized(int logicalindex, int oldsize, int newsize);

    void onComplexIndexDoubleClicked(const QModelIndex& index);

    void onMoveOutOfDataTable(int column);
    void onMoveOutOfComplexTable(int column);

    void onVBarValueChanged_DataTable(int value);

    void onDataTableClearSelection();
    void onComplexTableClearSelection();

    void onComplexTablePressed();
    void onDataTablePressed();

    void onSetDataTableSelectAll();
    void onDataChanged(const QModelIndex& index, const QString& section, const QVariant& val);
    void onEvaluate(const QModelIndex& index, const QString& expression);
    void onFilterOut(const QModelIndex& index, const FilterData& fd);
    void onFilterOut_(const QModelIndex& index, const FilterData& fd);
    void onResetColumn(const QModelIndex& index);
    void onHorizontalHeaderClicked(int index);
    void onDataCommitted(int type, const QModelIndex& index, const QVariant& oldValue,
                         const QVariant& newValue, const QVariant& extra);
    void onColumnDataChanged(int column, const ComData::ColumnBase* columnData);
    void onRowVisible(const QString& rowName, bool visible);

    void setTableRowVisible_(const QString &rowName, bool visible);
    void setTableElementVisible_(const QString& elem, bool show);
    void setColumnSignificance_(int column, ComData::ColumnBase::Significance sig);
    void setColumnVisible_(int column, bool visible);
    QModelIndexList setSelectedCellsMarked_(bool mark);
    void setCellsMarked(const QModelIndexList& indexList, bool mark);
    void removeColumn_(int column);
    ComData::ColumnBase* insertColumn_(int column, ComData::ColumnBase::ColumnType ct);
    void setColumnProperty_(const ColumnPropertyData& cpd);
    void swapColumn_(int from, int to);
    void moveColumn_(int from, int to);

    void updateMinSize();

private:
    BookTablePrivate* d_ptr = nullptr;
    friend class BookTablePrivate;

    // this is bad, shouldn't expose this kinda internal info
    // think out a better way to impl this
    friend class DeltaMaster;
    friend class RowVisibilityHandler;
    friend class ElementVisibilityHandler;
    friend class ColumnSignificanceHandler;
    friend class ColumnSignificancesHandler;
    friend class ColumnVisibilityHandler;
    friend class MarkCellHandler;
    friend class DeleteColumnHandler;
    friend class InsertColumnHandler;
    friend class AppendColumnHandler;
    friend class CustomFunctionHandler;
    friend class FilterHandler;
    friend class ColumnPropChangedHandler;
    friend class SwapColumnHandler;
    friend class MoveColumnHandler;
};

} // namespace
#endif // BOOKTABLE_H
