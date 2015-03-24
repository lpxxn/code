#ifndef DELTAMASTER_H
#define DELTAMASTER_H

#include <QObject>
#include <QAbstractItemModel>

#include <comdata/column.h>

#include "delta.h"

namespace ComData
{
    class Table;
}

namespace ComDataGui
{
class BookTable;
class DeltaBase;

class DeltaMaster
{
public:
    static void redo(ComData::Table* table, DeltaDataList& delta, BookTable* book = nullptr);
    static void undo(ComData::Table* table, DeltaDataList& delta, BookTable* book = nullptr);

    static DeltaDataList diff(ComData::Table* lhs, ComData::Table* rhs);
    static DeltaDataList dataCellDiff(ComData::Table* table, int row, int col, const QVariant& oldValue, const QVariant& newValue);
    static DeltaDataList complexCellDiff(ComData::Table* table, int row, int col, const QVariant& oldValue,
                                      const QVariant& newValue, const QString& section);
    static DeltaDataList rowVisibilityDiff(const QString& rowName, bool old, bool nw);
    static DeltaDataList elementVisibilityDiff(const QString& element, bool old, bool nw);
    static DeltaDataList significanceDiff(int column, ComData::ColumnBase::Significance oldSig,
                                          ComData::ColumnBase::Significance newSig);
    static DeltaDataList markCellDiff(const QModelIndexList& list, bool oldM, bool newM);

private:
    static DeltaDataList handleLargerTable(ComData::Table* lhs, ComData::Table* rhs);
    static DeltaDataList handleSmallerTable(ComData::Table* lhs, ComData::Table* rhs);
    static DeltaDataList handleEquivalentTable(ComData::Table* lhs, ComData::Table* rhs);
};

}
#endif // DELTAMASTER_H
