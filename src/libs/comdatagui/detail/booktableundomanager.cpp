#include "booktableundomanager.h"
#include "bookundocommand.h"

#include <QUndoStack>

namespace ComDataGui
{

BookTableUndoManager::BookTableUndoManager(BookTable *book, QObject* parent) :
    QObject(parent), m_bookTable(book)
{
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(UndoLimit);
}

BookTableUndoManager::~BookTableUndoManager()
{

}

void BookTableUndoManager::undo()
{
    m_undoStack->undo();
}

void BookTableUndoManager::redo()
{
    m_undoStack->redo();
}

void BookTableUndoManager::addCommand(const DeltaDataList& ddl)
{
    BookUndoCommand* cmd = new BookUndoCommand(m_bookTable, ddl);
    m_undoStack->push(cmd);
}

void BookTableUndoManager::addCommand(DeltaBase* delta)
{
    BookUndoCommand* cmd = new BookUndoCommand(m_bookTable, delta);
    m_undoStack->push(cmd);
}

} // namespace ComDataGui
