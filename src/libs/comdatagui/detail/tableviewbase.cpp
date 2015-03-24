#include "tableviewbase.h"

#include <QDebug>
#include <QMouseEvent>

namespace ComDataGui
{

static const qreal ZoomInDelta = 1.2;
static const qreal ZoomOutDelta = 0.8;

TableViewBase::TableViewBase(QWidget *parent) :
    QTableView(parent), m_lastRow(-1), m_zoomFactor(1.0)
{
}

void TableViewBase::resetLastRow()
{
    m_lastRow = -1;
}

void TableViewBase::setLastRow(int lastRow)
{
    m_lastRow = lastRow;
}

/*!
 * \reimp
 */
QModelIndex TableViewBase::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);
    emit customClearSelection();
    return current;
}

/*!
 * \brief 计算出选中区域的矩形
 * \return 选中区域的矩形
 */
Selection TableViewBase::selectionRangeRegion()
{
    QModelIndexList modelList = selectionModel()->selectedIndexes();
    Selection seRange;
    if (modelList.isEmpty())
        return seRange;
    qSort(modelList.begin(),modelList.end());

    seRange.startColIndex = modelList.first().column();
    seRange.endColIndex = modelList.first().column();

    seRange.startRowIndex = modelList.first().row();
    seRange.endRowIndex = modelList.last().row();

    foreach (const QModelIndex& mo, modelList) {
        seRange.endColIndex = qMax(mo.column(), seRange.endColIndex);
        seRange.startColIndex = qMin(mo.column(), seRange.startColIndex);
    }
    return seRange;
}

/*!
 * \reimp
 */
void TableViewBase::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::RightButton) {
        e->ignore();
    } else {
        emit pressed(); // prepared for resolving the bug
        QTableView::mousePressEvent(e);
    }
}

/*!
 * \reimp
 */
void TableViewBase::zoomIn()
{
    m_zoomFactor *= ZoomInDelta;

    for (int row = 0; row < model()->rowCount(); ++row)
        setRowHeight(row, rowHeight(row) * m_zoomFactor);
}

/*!
 * \reimp
 */
void TableViewBase::zoomOut()
{
    m_zoomFactor *= ZoomOutDelta;
    for (int row = 0; row < model()->rowCount(); ++row)
        setRowHeight(row, rowHeight(row) * m_zoomFactor);
}

/*!
 * \brief 设置放大倍数.
 * \param zoomFactor 放大倍数
 */
void TableViewBase::setZoomFactor(qreal zoomFactor)
{
    if (zoomFactor != m_zoomFactor) {
        m_zoomFactor = zoomFactor;
        for (int row = 0; row < model()->rowCount(); ++row)
            setRowHeight(row, rowHeight(row) * m_zoomFactor);
    }
}

/*!
 * \brief 获取放大倍数.
 * \return 放大倍数
 */
qreal TableViewBase::zoomFactor()
{
    return m_zoomFactor;
}

} // namespace ComDataGui
