#include "bookundocommand.h"
#include "../booktable.h"
#include "../constants.h"

#include <comdata/table.h>
namespace ComDataGui
{

BookUndoCommand::BookUndoCommand(BookTable *table, const DeltaDataList& d):
                                 m_tableWidget(table), m_delta(d)
{

}

BookUndoCommand::BookUndoCommand(BookTable *table, DeltaBase *delta) : m_tableWidget(table)
{
    m_delta.append(delta);
}

BookUndoCommand::~BookUndoCommand()
{
    qDeleteAll(m_delta);
    m_delta.clear();
}

/*!
 * \brief 发送列数据已改变信号, 便于更新缩略图等需要同步更新的数据.
 * \param table 表对象指针
 */
void emitDataUpdatedSignals(ComData::Table* table)
{
    for (int i = 0; i < table->size(); ++i)
        emit table->at(i)->dataChanged();
}

/*!
 * \reimp
 */
void BookUndoCommand::undo()
{
    ComData::Table* table = m_tableWidget->table();
    DeltaMaster::undo(table, m_delta, m_tableWidget);
    m_tableWidget->setTable(table);
}

/*!
 * \reimp
 */
void BookUndoCommand::redo()
{
    ComData::Table* table = m_tableWidget->table();
    DeltaMaster::redo(table, m_delta, m_tableWidget);
    m_tableWidget->setTable(table);
}

}
