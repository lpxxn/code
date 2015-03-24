#include "deltamaster.h"
#include "deltahandler.h"
#include "../constants.h"
#include "../booktable.h"

#include <comdata/table.h>

namespace ComDataGui
{

DeltaDataList DeltaMaster::diff(ComData::Table *lhs, ComData::Table *rhs)
{
    DeltaDataList ddl;
    if (lhs->size() > rhs->size())
        ddl = handleLargerTable(lhs, rhs);
    else if (lhs->size() < rhs->size())
        ddl = handleSmallerTable(lhs, rhs);
    else
        ddl = handleEquivalentTable(lhs, rhs);

    return ddl;
}

static inline void emitDataChangedSignals(ComData::Table* table)
{
    for (int i = 0; i < table->size(); ++i)
        emit table->at(i)->dataChanged();
}

void DeltaMaster::redo(ComData::Table *table, DeltaDataList &delta, BookTable* book)
{
    QScopedPointer<AbstractDeltaHandler> handler;

    // for boosting performance
    if (table)
        table->blockSignals(true);

    foreach (DeltaBase* db, delta) {
        handler.reset(DeltaHandlerFactory::instance()->createHandler((DeltaBase::Type)db->type()));
        if (!handler.isNull())
            handler->redo(db, book);
    }

    if (table) {
        table->blockSignals(false);
        emitDataChangedSignals(table);
    }
}

void DeltaMaster::undo(ComData::Table *table, DeltaDataList &delta, BookTable* book)
{
    QScopedPointer<AbstractDeltaHandler> handler;

    // for boosting performance
    if (table)
        table->blockSignals(true);

    foreach (DeltaBase* db, delta) {
        handler.reset(DeltaHandlerFactory::instance()->createHandler((DeltaBase::Type)db->type()));
        if (!handler.isNull())
            handler->undo(db, book);
    }

    if (table) {
        table->blockSignals(false);
        emitDataChangedSignals(table);
    }
}

DeltaDataList DeltaMaster::dataCellDiff(ComData::Table* table, int row, int col, const QVariant &oldValue, const QVariant &newValue)
{
    DeltaDataList ddl;
    ddl.append(DeltaData::create(table, col, row, oldValue, newValue));
    return ddl;
}

DeltaDataList DeltaMaster::complexCellDiff(ComData::Table* table, int row, int col, const QVariant &oldValue,
                                       const QVariant &newValue, const QString &section)
{
    DeltaDataList ddl;
    ddl.append(DeltaComplex::create(table, section, col, row, oldValue, newValue));
    return ddl;
}

DeltaDataList DeltaMaster::rowVisibilityDiff(const QString& rowName, bool old, bool nw)
{
    DeltaDataList ddl;
    ddl.append(DeltaRowVisibility::create(old, nw, rowName));
    return ddl;
}

DeltaDataList DeltaMaster::elementVisibilityDiff(const QString& element, bool old, bool nw)
{
    DeltaDataList ddl;
    ddl.append(DeltaElementVisibility::create(old, nw, element));
    return ddl;
}

DeltaDataList DeltaMaster::significanceDiff(int column, ComData::ColumnBase::Significance oldSig, ComData::ColumnBase::Significance newSig)
{
    DeltaDataList ddl;
    ddl.append(DeltaSetSignificance::create(column, oldSig, newSig));
    return ddl;
}

DeltaDataList DeltaMaster::markCellDiff(const QModelIndexList &list, bool oldM, bool newM)
{
    DeltaDataList ddl;
    ddl.append(DeltaMarkCell::create(list, oldM, newM));
    return ddl;
}

DeltaDataList DeltaMaster::handleLargerTable(ComData::Table* lhs, ComData::Table* rhs)
{
    DeltaDataList ddl;

    int column = 0;
    for (column = 0; column < rhs->size(); ++column) {
        QSharedPointer<ComData::ColumnBase> ori = lhs->at(column);
        QSharedPointer<ComData::ColumnBase> inc = rhs->at(column);

        Q_ASSERT(ori->size() == inc->size());

        for (int row = 0; row < ori->size(); ++row) {
            if (ori->cell(row) != inc->cell(row))
                ddl.append(DeltaData::create(rhs, column, row, ori->cell(row), inc->cell(row)));
        }
    }

    for ( ; column < lhs->size(); ++column) {
        QSharedPointer<ComData::ColumnBase> colData = lhs->at(column);
        for (int row = 0; row < colData->size(); ++row)
            ddl.append(DeltaData::create(rhs, column, row, QVariant(), colData->cell(row)));
    }

    return ddl;
}

DeltaDataList DeltaMaster::handleSmallerTable(ComData::Table* lhs, ComData::Table* rhs)
{
    DeltaDataList ddl;

    int column = 0;
    for (column = 0; column < lhs->size(); ++column) {
        QSharedPointer<ComData::ColumnBase> ori = rhs->at(column);
        QSharedPointer<ComData::ColumnBase> inc = lhs->at(column);

        Q_ASSERT(ori->size() == inc->size());

        for (int row = 0; row < ori->size(); ++row)
            if (ori->cell(row) != inc->cell(row))
                ddl.append(DeltaData::create(rhs, column, row, ori->cell(row), inc->cell(row)));

    }

    for ( ; column < rhs->size(); ++column) {
        QSharedPointer<ComData::ColumnBase> colData = rhs->at(column);
        for (int row = 0; row < colData->size(); ++row)
            ddl.append(DeltaData::create(rhs, column, row, QVariant(), colData->cell(row)));

    }

    return ddl;
}

DeltaDataList DeltaMaster::handleEquivalentTable(ComData::Table* lhs, ComData::Table* rhs)
{
    DeltaDataList ddl;

    for (int column = 0; column < rhs->size(); ++column) {
        QSharedPointer<ComData::ColumnBase> ori = lhs->at(column);
        QSharedPointer<ComData::ColumnBase> inc = rhs->at(column);

        if (ori->size() < inc->size()) {
            for (int row = 0; row < inc->size(); ++row)
                ddl.append(DeltaData::create(rhs, column, row, QVariant(), inc->cell(row)));

        } else if (inc->size() == 0) {
            // if cleared
            for (int row = 0; row < ori->size(); ++row)
                ddl.append(DeltaData::create(rhs, column, row, ori->cell(row), QVariant()));

        } else if (ori->size() > inc->size()){
            for (int row = 0; row < ori->size(); ++row)
                if (ori->cell(row) != inc->cell(row))
                    ddl.append(DeltaData::create(rhs, column, row, ori->cell(row), inc->cell(row)));
        } else if (ori->size() == inc->size()) {
            for (int row = 0; row < ori->size(); ++row) {
                if (ori->cell(row) != inc->cell(row))
                    ddl.append(DeltaData::create(rhs, column, row, ori->cell(row), inc->cell(row)));
            }
        }
    }

    return ddl;
}

}
