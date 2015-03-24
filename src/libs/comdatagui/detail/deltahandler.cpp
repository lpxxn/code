#include "deltahandler.h"
#include "deltamaster.h"
#include "../booktable.h"
#include "../constants.h"

#include <comdata/table.h>

#include <QCoreApplication>

namespace ComDataGui
{

DeltaHandlerFactory::DeltaHandlerFactory()
{

}

DeltaHandlerFactory* DeltaHandlerFactory::instance()
{
    static DeltaHandlerFactory _instance;
    return &_instance;
}

template <class T>
AbstractDeltaHandler* create_handler()
{
    return new T;
}

AbstractDeltaHandler* DeltaHandlerFactory::createHandler(DeltaBase::Type type)
{
    AbstractDeltaHandler* handler = nullptr;
    switch (type) {

#define INIT_HANDLER(ENUM, NAME) \
    case ENUM:\
        handler = create_handler<NAME>(); \
        break; \

    INIT_HANDLER(DeltaBase::Data, DataHandler);
    INIT_HANDLER(DeltaBase::Complex, ComplexHandler);
    INIT_HANDLER(DeltaBase::RowVisibility, RowVisibilityHandler);
    INIT_HANDLER(DeltaBase::ElementVisibility, ElementVisibilityHandler);
    INIT_HANDLER(DeltaBase::SetSignificance, ColumnSignificanceHandler);
    INIT_HANDLER(DeltaBase::SetSignificances, ColumnSignificancesHandler);
    INIT_HANDLER(DeltaBase::ColumnVisibility, ColumnVisibilityHandler);
    INIT_HANDLER(DeltaBase::MarkCell, MarkCellHandler);
    INIT_HANDLER(DeltaBase::DeleteColumn, DeleteColumnHandler);
    INIT_HANDLER(DeltaBase::InsertColumn, InsertColumnHandler);
    INIT_HANDLER(DeltaBase::AppendColumn, AppendColumnHandler);
    INIT_HANDLER(DeltaBase::CustomFunction, CustomFunctionHandler);
    INIT_HANDLER(DeltaBase::Filter, FilterHandler);
    INIT_HANDLER(DeltaBase::AppendRow, AppendRowHandler);
    INIT_HANDLER(DeltaBase::InsertRow, InsertRowHandler);
    INIT_HANDLER(DeltaBase::ColumnPropChanged, ColumnPropChangedHandler);
    INIT_HANDLER(DeltaBase::ClearTable, ClearTableHandler);
    INIT_HANDLER(DeltaBase::SwapColumn, SwapColumnHandler);
    INIT_HANDLER(DeltaBase::MoveColumn, MoveColumnHandler);

    default:
        break;
    }

#undef INIT_HANDLER
    return handler;
}

void DataHandler::redo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaData* dd = dynamic_cast<DeltaData*>(delta);
    QSharedPointer<ComData::ColumnBase> colData = delta->table->at(dd->column);
    colData->setCell(dd->row, dd->newValue);
}

void DataHandler::undo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaData* dd = dynamic_cast<DeltaData*>(delta);
    QSharedPointer<ComData::ColumnBase> colData = delta->table->at(dd->column);
    colData->setCell(dd->row, dd->oldValue);
}

void ComplexHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaComplex* dc = dynamic_cast<DeltaComplex*>(delta);
    QSharedPointer<ComData::ColumnBase> colData = delta->table->at(dc->column);
    if (!colData.isNull()) {
        if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME))) {
            colData->setLongName(dc->newValue.toString());
        } else if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT))) {
            colData->setUnit(dc->newValue.toString());
        } else if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT))) {
            colData->setComment(dc->newValue.toString());
        }
    }

    if (book)
        book->setComplexData(dc->row, dc->column, dc->newValue);
}

void ComplexHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaComplex* dc = dynamic_cast<DeltaComplex*>(delta);
    QSharedPointer<ComData::ColumnBase> colData = delta->table->at(dc->column);
    if (!colData.isNull()) {
        if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME))) {
            colData->setLongName(dc->oldValue.toString());
        } else if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT))) {
            colData->setUnit(dc->oldValue.toString());
        } else if (!dc->section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT))) {
            colData->setComment(dc->oldValue.toString());
        }
    }

    if (book)
        book->setComplexData(dc->row, dc->column, dc->oldValue);
}

void RowVisibilityHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaRowVisibility* drv = dynamic_cast<DeltaRowVisibility*>(delta);
    book->setTableRowVisible_(drv->rowName, drv->newVisible);
}

void RowVisibilityHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaRowVisibility* drv = dynamic_cast<DeltaRowVisibility*>(delta);
    book->setTableRowVisible_(drv->rowName, drv->oldVisible);
}

void ElementVisibilityHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaElementVisibility* drv = dynamic_cast<DeltaElementVisibility*>(delta);
    book->setTableElementVisible_(drv->element, drv->newVisible);
}

void ElementVisibilityHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaElementVisibility* drv = dynamic_cast<DeltaElementVisibility*>(delta);
    book->setTableElementVisible_(drv->element, drv->oldVisible);
}

void ColumnSignificanceHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaSetSignificance* dss = dynamic_cast<DeltaSetSignificance*>(delta);
    book->setColumnSignificance_(dss->column, dss->newSig);
}

void ColumnSignificanceHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaSetSignificance* dss = dynamic_cast<DeltaSetSignificance*>(delta);
    book->setColumnSignificance_(dss->column, dss->oldSig);
}

void ColumnSignificancesHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaSetSignificances* dss = dynamic_cast<DeltaSetSignificances*>(delta);
    QMap<int, QPair<ComData::ColumnBase::Significance, ComData::ColumnBase::Significance> >::const_iterator
            iter = dss->columnSigs.begin();
    while (iter != dss->columnSigs.end()) {
        book->setColumnSignificance_(iter.key(), iter.value().second);
        ++iter;
    }
}

void ColumnSignificancesHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaSetSignificances* dss = dynamic_cast<DeltaSetSignificances*>(delta);
    QMap<int, QPair<ComData::ColumnBase::Significance, ComData::ColumnBase::Significance> >::const_iterator
            iter = dss->columnSigs.begin();
    while (iter != dss->columnSigs.end()) {
        book->setColumnSignificance_(iter.key(), iter.value().first);
        ++iter;
    }
}

void ColumnVisibilityHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaColumnVisibility* dcv = dynamic_cast<DeltaColumnVisibility*>(delta);
    for (int col : dcv->columns)
        book->setColumnVisible_(col, dcv->newVisible);
}

void ColumnVisibilityHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaColumnVisibility* dcv = dynamic_cast<DeltaColumnVisibility*>(delta);
    for (int col : dcv->columns)
        book->setColumnVisible_(col, dcv->oldVisible);
}

void MarkCellHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaMarkCell* dmc = dynamic_cast<DeltaMarkCell*>(delta);
    book->setCellsMarked(dmc->indexList, dmc->newMark);
}

void MarkCellHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaMarkCell* dmc = dynamic_cast<DeltaMarkCell*>(delta);
    book->setCellsMarked(dmc->indexList, dmc->oldMark);
}

static inline bool larger(int lhs, int rhs)
{
    return lhs > rhs;
}

void DeleteColumnHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaDeleteColumn* ddc = dynamic_cast<DeltaDeleteColumn*>(delta);
    QList<int> columns = ddc->columnDataMap.keys();
    qSort(columns.begin(), columns.end(), larger);

    foreach (int col, columns)
        book->removeColumn_(col);
}

void DeleteColumnHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaDeleteColumn* ddc = dynamic_cast<DeltaDeleteColumn*>(delta);

    QMap<int, ComData::ColumnBase*>::iterator iter = ddc->columnDataMap.begin();
    while (iter != ddc->columnDataMap.end()) {
        book->table()->insert(iter.key(), QSharedPointer<ComData::ColumnBase>(iter.value()->clone()));
        ++iter;
    }
}

void InsertColumnHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaInsertColumn* dic = dynamic_cast<DeltaInsertColumn*>(delta);
    book->insertColumn_(dic->column, dic->columnData->type());
}

void InsertColumnHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaInsertColumn* dic = dynamic_cast<DeltaInsertColumn*>(delta);
    book->removeColumn_(dic->column);
}

void AppendColumnHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaAppendColumn* dac = dynamic_cast<DeltaAppendColumn*>(delta);
    book->addColumn(dac->columnData->clone());
}

void AppendColumnHandler::undo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(delta)
    int columnCount = book->columnList().size();
    book->removeColumn_(columnCount - 1);
}

void CustomFunctionHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaCustomFunction* dcf = dynamic_cast<DeltaCustomFunction*>(delta);
    if (dcf->columnData) {
        dcf->columnData->setExpression(dcf->newFunction);
        DeltaMaster::redo(dcf->table, dcf->dataList, book);
    }
}

void CustomFunctionHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaCustomFunction* dcf = dynamic_cast<DeltaCustomFunction*>(delta);
    if (dcf->columnData) {
        dcf->columnData->setExpression(dcf->oldFunction);
        DeltaMaster::undo(dcf->table, dcf->dataList, book);
    }
}

void FilterHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaFilter* df = dynamic_cast<DeltaFilter*>(delta);
    book->onFilterOut_(df->index, df->newFilter);
    DeltaMaster::redo(df->table, df->dataList, book);
}

void FilterHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaFilter* df = dynamic_cast<DeltaFilter*>(delta);
    book->onFilterOut_(df->index, df->oldFilter);
    DeltaMaster::undo(df->table, df->dataList, book);
}

void AppendRowHandler::redo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaAppendRow* dar = dynamic_cast<DeltaAppendRow*>(delta);
    dar->table->appendRow();
}

void AppendRowHandler::undo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaAppendRow* dar = dynamic_cast<DeltaAppendRow*>(delta);
    dar->table->removeRow(dar->table->rowCount());
}

void InsertRowHandler::redo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaInsertRow* dir = dynamic_cast<DeltaInsertRow*>(delta);
    dir->table->insertRow(dir->row);
}

void InsertRowHandler::undo(DeltaBase *delta, BookTable *book) const
{
    Q_UNUSED(book)
    DeltaInsertRow* dir = dynamic_cast<DeltaInsertRow*>(delta);
    dir->table->removeRow(dir->row);
}

void ColumnPropChangedHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaColumnPropChanged* dcpc = dynamic_cast<DeltaColumnPropChanged*>(delta);
    if (dcpc->newProp.isValid())
        book->setColumnProperty_(dcpc->newProp);
}

void ColumnPropChangedHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaColumnPropChanged* dcpc = dynamic_cast<DeltaColumnPropChanged*>(delta);
    if (dcpc->oldProp.isValid())
        book->setColumnProperty_(dcpc->oldProp);
}

void ClearTableHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaClearTable* dct = dynamic_cast<DeltaClearTable*>(delta);
    for (int i = 0; i < dct->newTable->size(); ++i)
        dct->newTable->at(i)->clear();
    book->update();
}

void ClearTableHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaClearTable* dct = dynamic_cast<DeltaClearTable*>(delta);
    book->setTable(dct->oldTable);
}

void SwapColumnHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaSwapColumn* dsc = dynamic_cast<DeltaSwapColumn*>(delta);
    book->swapColumn_(dsc->from, dsc->to);
}

void SwapColumnHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaSwapColumn* dsc = dynamic_cast<DeltaSwapColumn*>(delta);
    book->swapColumn_(dsc->to, dsc->from);
}

void MoveColumnHandler::redo(DeltaBase *delta, BookTable *book) const
{
    DeltaMoveColumn* dsc = dynamic_cast<DeltaMoveColumn*>(delta);
    book->moveColumn_(dsc->from, dsc->to);
}

void MoveColumnHandler::undo(DeltaBase *delta, BookTable *book) const
{
    DeltaMoveColumn* dsc = dynamic_cast<DeltaMoveColumn*>(delta);
    book->moveColumn_(dsc->to, dsc->from);
}

}
