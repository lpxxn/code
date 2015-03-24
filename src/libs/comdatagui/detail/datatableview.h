#ifndef DATATABLEVIEW_H
#define DATATABLEVIEW_H

#include "../comdatagui_global.h"
#include "tableviewbase.h"

namespace ComDataGui
{
class DataTableView : public TableViewBase
{
    Q_OBJECT
public:
    explicit DataTableView(QWidget *parent = 0);

protected:
    virtual void commitData(QWidget* editor);
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

};
} // namespace ComDataGui


#endif // DATATABLEVIEW_H
