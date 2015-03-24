#ifndef BASETABLEVIEW_H
#define BASETABLEVIEW_H

#include <QTableView>
#include <QTimer>

namespace ComDataGui
{

/*!
 * \brief The SelectRangeValue struct
 * 选中矩形区域的开始index和结束index
 */
struct Selection
{
    bool isValid() {
        return !(startRowIndex == -1
                || startColIndex == -1
                || endRowIndex == -1
                || endColIndex == -1);
    }

    int startRowIndex = -1;
    int startColIndex = -1;
    int endRowIndex = -1;
    int endColIndex = -1;
};

class TableViewBase : public QTableView
{
    Q_OBJECT
public:
    enum Type { Complex, Data };
    explicit TableViewBase(QWidget *parent = 0);
    virtual ~TableViewBase() {}

    void resetLastRow();
    void setLastRow(int lastRow);

    Selection selectionRangeRegion();

    virtual void zoomIn();
    virtual void zoomOut();

    void setZoomFactor(qreal zoomFactor);
    qreal zoomFactor();

signals:
    void moveOutFromColumn(int column);
    void customClearSelection();
    void pressed();
    void dataCommitted(int type, const QModelIndex& index,
                      const QVariant& oldValue, const QVariant& newValue, const QVariant& extra);


protected:
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    virtual void mousePressEvent(QMouseEvent* e);

protected:
    int m_lastRow;
    qreal m_zoomFactor;
};

} // namespace ComDataGui

#endif // BASETABLEVIEW_H
