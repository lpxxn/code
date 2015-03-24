#ifndef BOOKTABLEUNDOMANAGER_H
#define BOOKTABLEUNDOMANAGER_H

#include <QObject>

#include "delta.h"

class QUndoStack;
namespace ComDataGui
{

class BookUndoCommand;
class BookTable;
class BookTableUndoManager : public QObject
{
    Q_OBJECT
public:
    explicit BookTableUndoManager(BookTable *book, QObject* parent = 0);
    ~BookTableUndoManager();

    enum { UndoLimit = 20 };

public slots:
    void undo();
    void redo();

    void addCommand(const DeltaDataList& ddl);
    void addCommand(DeltaBase* delta);

private:
    BookTable* m_bookTable;
    QUndoStack* m_undoStack;
};

} // namespace
#endif // BOOKTABLEUNDOMANAGER_H
