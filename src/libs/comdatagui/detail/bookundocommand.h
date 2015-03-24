#ifndef BOOKUNDOCOMMAND_H
#define BOOKUNDOCOMMAND_H

#include <QUndoCommand>
#include <comdata/column.h>
#include <QAbstractItemModel>

#include "deltamaster.h"

namespace ComData
{
    class Table;
}

namespace ComDataGui
{

class BookTable;

class BookUndoCommand : public QUndoCommand
{
public:
    explicit BookUndoCommand(BookTable* table, const DeltaDataList& d);
    explicit BookUndoCommand(BookTable* table, DeltaBase* delta);
    virtual ~BookUndoCommand();

    virtual void undo();
    virtual void redo();

protected:
    BookTable* m_tableWidget;
    DeltaDataList m_delta;
};

}
#endif // BOOKUNDOCOMMAND_H
