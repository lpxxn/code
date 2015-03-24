#ifndef DELTAHANDLER_H
#define DELTAHANDLER_H

#include "delta.h"

namespace ComDataGui
{
class BookTable;
class AbstractDeltaHandler
{
public:
    virtual void redo(DeltaBase* delta, BookTable* book) const = 0;
    virtual void undo(DeltaBase* delta, BookTable* book) const = 0;
};

class DeltaHandlerFactory
{
public:
    static DeltaHandlerFactory* instance();
    AbstractDeltaHandler* createHandler(DeltaBase::Type type);

private:
    explicit DeltaHandlerFactory();
    Q_DISABLE_COPY(DeltaHandlerFactory)
};

#define DECLARE_HANDLER(NAME) class NAME##Handler : public AbstractDeltaHandler \
                              { \
                                public: \
                                 void redo(DeltaBase *delta, BookTable *book) const; \
                                 void undo(DeltaBase *delta, BookTable *book) const; \
                              }; \

DECLARE_HANDLER(Data)
DECLARE_HANDLER(Complex)
DECLARE_HANDLER(RowVisibility)
DECLARE_HANDLER(ElementVisibility)
DECLARE_HANDLER(ColumnSignificance)
DECLARE_HANDLER(ColumnSignificances)
DECLARE_HANDLER(ColumnVisibility)
DECLARE_HANDLER(MarkCell)
DECLARE_HANDLER(DeleteColumn)
DECLARE_HANDLER(InsertColumn)
DECLARE_HANDLER(AppendColumn)
DECLARE_HANDLER(CustomFunction)
DECLARE_HANDLER(Filter)
DECLARE_HANDLER(AppendRow)
DECLARE_HANDLER(InsertRow)
DECLARE_HANDLER(ColumnPropChanged)
DECLARE_HANDLER(ClearTable)
DECLARE_HANDLER(SwapColumn)
DECLARE_HANDLER(MoveColumn)

}
#endif // DELTAHANDLER_H
