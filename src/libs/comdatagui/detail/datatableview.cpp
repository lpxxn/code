#include "datatableview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

namespace ComDataGui
{

DataTableView::DataTableView(QWidget *parent) :
    TableViewBase(parent)
{
}

template <class Widget>
class Editor
{
public:
    static QVariant value(QWidget* widget, bool& ok)
    {
        Widget* sb = qobject_cast<Widget*>(widget);
        if (sb) {
            ok = true;
            return sb->value();
        } else {
            ok = false;
            return QVariant();
        }
    }
};

template <>
class Editor<QLineEdit>
{
public:
    static QVariant value(QWidget* widget, bool& ok)
    {
        QLineEdit* le = qobject_cast<QLineEdit*>(widget);
        if (le) {
            ok = true;
            return le->text();
        } else {
            ok = false;
            return QVariant();
        }
    }
};

template <>
class Editor<QDateTimeEdit>
{
public:
    static QVariant value(QWidget* widget, bool& ok)
    {
        QDateTimeEdit* dte = qobject_cast<QDateTimeEdit*>(widget);
        if (dte) {
            ok = true;
            return dte->dateTime();
        } else {
            ok = false;
            return QVariant();
        }
    }
};

/*!
 * \reimp
 */
void DataTableView::commitData(QWidget *editor)
{
    QModelIndex index = currentIndex();
    QVariant oldValue = model()->data(index);
    QVariant newValue;

    bool ok = false;
    newValue = Editor<QLineEdit>::value(editor, ok);

    if (!ok)
        newValue = Editor<QDateTimeEdit>::value(editor, ok);

    if (!ok)
        newValue = Editor<QSpinBox>::value(editor, ok);

    if (!ok)
        newValue = Editor<QDoubleSpinBox>::value(editor, ok);

    emit dataCommitted(Data, index, oldValue, newValue, QVariant());
    TableViewBase::commitData(editor);
}

/*!
 * \reimp
 */
QModelIndex DataTableView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
      QModelIndex current = TableViewBase::moveCursor(cursorAction, modifiers);

      if (cursorAction == MoveUp) {
          if (m_lastRow == 0) {
              emit moveOutFromColumn(current.column());
              return current;
          }
          m_lastRow = current.row();
      } else if (cursorAction == MoveDown){
          m_lastRow = -1;
      }

      return current;
}
} // namespace ComDataGui
