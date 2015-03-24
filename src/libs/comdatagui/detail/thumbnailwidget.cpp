#include "thumbnailwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTableView>
#include <QHeaderView>

#include <comdata/column.h>

namespace ComDataGui
{

static const int PixmapHeight = 40;

ThumbnailWidget::ThumbnailWidget(ComData::ColumnBase* colData, QWidget *parent) :
    QLabel(parent), m_columnData(colData)
{
    connect(colData, SIGNAL(dataChanged()), this, SLOT(onColumnDataChanged()));
}

/*!
 * \brief 根据传递的数据生成曲线缩略图.
 * \param colData 列数据
 * \param size 缩略图大小
 */
QPixmap ThumbnailWidget::preview(const ComData::ColumnBase* colData, const QSize &size)
{
    QPixmap pixmap(size.width(), size.height());
    pixmap.fill(Qt::transparent);

    if (colData->size() != 0) {
        QPainter painter(&pixmap);
        if (!painter.isActive())
            return pixmap;

        double min = 0;
        if (colData->size() != 0)
            min = colData->cell(0).toDouble();
        double max = 0;
        for (int i = 0; i < colData->size(); ++i) {
            if (min > colData->cell(i).toDouble())
                min = colData->cell(i).toDouble();
            if (max < colData->cell(i).toDouble())
                max = colData->cell(i).toDouble();
        }

        // draw curve
        painter.setPen(QPen(Qt::black, 1));

        QVector<QPointF> pots;

        qreal range = max - min;
        int extra = 10;

        if (range != 0) {
            int contentHeight = pixmap.height() - 2 * extra;
            int contentWidth = pixmap.width() - 2 * extra;
            qreal incre = (qreal) contentWidth / colData->size();
            qreal startX = extra;

            for (int index = 0; index < colData->size()- 1; ++index) {
                qreal value = colData->cell(index).toDouble();
                qreal dY1 = (qreal)contentHeight / range * value;
                qreal y1 = contentHeight - dY1 + extra;
                qreal nextVal = colData->cell(index+1).toDouble();
                qreal dY2 = (qreal)contentHeight / range * nextVal;
                qreal y2 = contentHeight - dY2 + extra;
                QPointF dataPot1(startX, y1);
                QPointF dataPot2(startX + incre, y2);
                startX += incre;
                pots.push_back(dataPot1);
                pots.push_back(dataPot2);
            }

            painter.drawLines(pots);
        }

        painter.end();
    }

    return pixmap;
}

void ThumbnailWidget::setTable(QTableView* table)
{
    if (m_table != table) {
        m_table = table;
        connect(m_table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(onHorizontalColumnResized(int,int,int)));
        connect(m_table->verticalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(onVerticalColumnResized(int,int,int)));
    }
}

void ThumbnailWidget::setRow(int row)
{
    m_row = row;
}

void ThumbnailWidget::setIndex(const QModelIndex &index)
{
    m_index = index;
}

void ThumbnailWidget::setColumnData(ComData::ColumnBase* colData)
{
    if (m_columnData != colData) {
        m_columnData = colData;
        connect(colData, SIGNAL(dataChanged()), this, SLOT(onColumnDataChanged()));
    }
}

ComData::ColumnBase* ThumbnailWidget::columnData() const
{
    return m_columnData;
}

void ThumbnailWidget::onHorizontalColumnResized(int column, int oldWidth, int newWidth)
{
    Q_UNUSED(column)
    Q_UNUSED(oldWidth)
    Q_UNUSED(newWidth)

    const QRect r = m_table->visualRect(m_index);
    if (r.isValid()) {
        QPixmap pixmap = preview(m_columnData, r.size());
        setPixmap(pixmap);
    }
}

void ThumbnailWidget::onVerticalColumnResized(int column, int oldWidth, int newWidth)
{
    Q_UNUSED(oldWidth)
    Q_UNUSED(newWidth)

    if (column != m_row)
        return;

    const QRect r = m_table->visualRect(m_index);
    QPixmap pixmap = preview(m_columnData, r.size());
    setPixmap(pixmap);
}

void ThumbnailWidget::onColumnDataChanged()
{
    const QRect r = m_table->visualRect(m_index);
    QPixmap pixmap = ThumbnailWidget::preview(m_columnData, r.size());
    setPixmap(pixmap);
}

}
