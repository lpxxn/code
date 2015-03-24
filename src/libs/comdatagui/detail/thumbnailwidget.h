#ifndef THUMBNAILWIDGET_H
#define THUMBNAILWIDGET_H

#include <QLabel>
#include <QModelIndex>

class QPainter;
class QPaintEvent;
class QTableView;

namespace ComData
{
    class ColumnBase;
}

namespace ComDataGui
{

/*!
 * \class 缩略图类.
 */
class ThumbnailWidget : public QLabel
{
    Q_OBJECT
public:
    explicit ThumbnailWidget(ComData::ColumnBase* colData, QWidget *parent = 0);

    static QPixmap preview(const ComData::ColumnBase* colData, const QSize& size);

    void setTable(QTableView* table);
    void setRow(int row);
    void setIndex(const QModelIndex& index);

    void setColumnData(ComData::ColumnBase* colData);
    ComData::ColumnBase* columnData() const;

private slots:
    void onHorizontalColumnResized(int column, int oldWidth, int newWidth);
    void onVerticalColumnResized(int column, int oldWidth, int newWidth);
    void onColumnDataChanged();

private:
    ComData::ColumnBase* m_columnData = nullptr;
    int m_row = -1;
    QModelIndex m_index;
    QTableView* m_table = nullptr;
};

}
#endif // THUMBNAILWIDGET_H
