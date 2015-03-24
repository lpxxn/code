#include "complextableview.h"
#include "ui_filterdialog.h"
#include "ui_advancedfunctiondialog.h"
#include "thumbnailwidget.h"
#include "filterdialog.h"
#include "advancedfunctiondialog.h"
#include "../booktable.h"

#include <vizcore/vizconstants.h>
#include <comdata/column.h>
#include <comdatagui/constants.h>

#include <QDebug>
#include <QLineEdit>
#include <QStringListModel>
#include <QHeaderView>
#include <QSpinBox>
#include <QLabel>
#include <QTextEdit>
#include <QDateEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>

namespace ComDataGui
{

ComplexTableView::ComplexTableView(QWidget *parent) : TableViewBase(parent), m_filterDlg(nullptr)
{
    init();
}

/*!
 * \brief 设置列的长名称.
 * \param column 列索引
 * \param longName 长名称
 */
void ComplexTableView::setLongName(int column, const QString& longName)
{
    setCellData(column, ComDataGui::Constants::TABLEROWLONGNAME, longName);
}

/*!
 * \brief 设置列的单位.
 * \param column 列索引
 * \param unit 单位
 */
void ComplexTableView::setUnit(int column, const QString& unit)
{
    setCellData(column, ComDataGui::Constants::TABLEROWUNIT, unit);
}

/*!
 * \brief 交换列.
 * \param from 源列索引
 * \param to 目标列索引
 */
void ComplexTableView::swapColumn(int from, int to)
{
    int row = rowOfLabel(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    QModelIndex fromIndex = model()->index(row, from);
    QModelIndex toIndex = model()->index(row, to);

    setIndexWidget(fromIndex, nullptr);
    setIndexWidget(toIndex, nullptr);

    m_columnThumbnailMap.remove(from);
    m_columnThumbnailMap.remove(to);
}

/*!
 * \brief 移动列.
 * \param from 源列索引
 * \param to 目标列索引
 */
void ComplexTableView::moveColumn(int from, int to)
{
    int row = rowOfLabel(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    QModelIndex fromIndex = model()->index(row, from);
    QModelIndex toIndex = model()->index(row, to);

    setIndexWidget(fromIndex, nullptr);
    setIndexWidget(toIndex, nullptr);

    m_columnThumbnailMap.remove(from);
    m_columnThumbnailMap.remove(to);
}

void ComplexTableView::onColumnDataChanged(int column, const ComData::ColumnBase *columnData)
{
    int thumbnailRow = rowOfLabel(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    if (thumbnailRow < 0)
        return;

    QModelIndex index = model()->index(thumbnailRow, column);
    if (!m_columnThumbnailMap.contains(column)) {
        ThumbnailWidget* thumbnail = new ThumbnailWidget(const_cast<ComData::ColumnBase*>(columnData));
        thumbnail->setRow(thumbnailRow);
        thumbnail->setTable(this);
        thumbnail->setIndex(index);
        setIndexWidget(index, thumbnail);
        m_columnThumbnailMap.insert(column, thumbnail);
    } else {
        if (m_columnThumbnailMap.value(column)->columnData() != columnData) {
            qDebug() << __FUNCTION__ << "reset column data.";
            m_columnThumbnailMap[column]->setColumnData(const_cast<ComData::ColumnBase*>(columnData));
        }
    }

    QRect r = visualRect(index);
    QPixmap pixmap = ThumbnailWidget::preview(columnData, r.size());
    m_columnThumbnailMap[column]->setPixmap(pixmap);
}

/*!
 * \brief 删除所有缩略图.
 */
void ComplexTableView::removeThumbnails()
{
    int row = rowOfLabel(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    for (int col = 0; col < model()->columnCount(); ++col) {
        QModelIndex mi = model()->index(row, col);
        setIndexWidget(mi, nullptr);
    }

    QList<ThumbnailWidget*> thumbnails = m_columnThumbnailMap.values();
    qDeleteAll(thumbnails);

    m_columnThumbnailMap.clear();
}

/*!
 * \reimp
 */
QModelIndex ComplexTableView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = TableViewBase::moveCursor(cursorAction, modifiers);

    const int lastRow = model()->rowCount() - 1;
    if (cursorAction == MoveDown) {
        if (m_lastRow == lastRow) {
            emit moveOutFromColumn(current.column());
            return current;
        }
        m_lastRow = current.row();
    } else if (cursorAction == MoveUp){
        m_lastRow = -1;
    }

    return current;
}

/*!
 * \reimp
 */
void ComplexTableView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Delete) {
        QModelIndex index = currentIndex();
        QString section = model()->headerData(index.row(), Qt::Vertical, Qt::DisplayRole).toString();

        if (section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)) == 0)
            emit evaluate(index, QString());
        else if (section.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER)) == 0)
            emit resetColumn(index);
    }
}

/*!
 * \reimp
 */
void ComplexTableView::columnResized(int column, int oldWidth, int newWidth)
{
    Q_UNUSED(column)
    Q_UNUSED(oldWidth)
    Q_UNUSED(newWidth)
    emit updateMinSize();
}

void ComplexTableView::onIndexDoubleClicked(const QModelIndex &index)
{
    m_pressedIndex = index;
    QString verticalHeaderLabel = model()->headerData(index.row(), Qt::Vertical, Qt::DisplayRole).toString();

    if (verticalHeaderLabel.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER)) == 0) {
        // pop up filter dialog
        if (m_filterDlg == nullptr) {
            m_filterDlg = new FilterDialog(this);
            connect(m_filterDlg, SIGNAL(setFilter(FilterData)), this, SLOT(onSetFilter(FilterData)));
        }

        m_filterDlg->reset();
        QRect geo = visualRect(index);
        QPoint pos = mapToGlobal(geo.bottomRight());
        m_filterDlg->move(pos);
        m_filterDlg->show();
    } else if (verticalHeaderLabel.compare(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)) == 0)
        showAdvancedDialog();
}

void ComplexTableView::onSetFilter(const FilterData& fd)
{
    emit filterOut(currentIndex(), fd);

    const QString strFilter = fd.toString();
    model()->setData(m_pressedIndex, strFilter, Qt::DisplayRole);
    update(m_pressedIndex);

    emit setFilter(m_pressedIndex, strFilter);
}

/*!
 * \brief 显示设置用户函数对话框.
 */
void ComplexTableView::showAdvancedDialog()
{
    if (!m_advancedFuncDlg) {
        m_advancedFuncDlg = new AdvancedFunctionDialog(this);
        connect(m_advancedFuncDlg, SIGNAL(setExpression(QString)), this, SLOT(onSetExpression(QString)));
    }

    if (m_filterDlg && m_filterDlg->isVisible())
        m_filterDlg->hide();

    BookTable* bt = qobject_cast<BookTable*>(parent());
    m_advancedFuncDlg->setColumnList(bt->columnList());
    m_advancedFuncDlg->setText(model()->itemData(currentIndex()).value(Qt::DisplayRole).toString());
    m_advancedFuncDlg->show();
}

void ComplexTableView::onSetExpression(const QString &exp)
{
    QModelIndex index = currentIndex();
    model()->setData(index, exp);
    emit evaluate(index, exp);
}

/*!
 * \reimp
 */
void ComplexTableView::commitData(QWidget *editor)
{
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    QModelIndex index = currentIndex();
    QString section = model()->headerData(index.row(), Qt::Vertical, Qt::DisplayRole).toString();

    QVariant oldValue = model()->data(index);
    QVariant newValue(le->text());
    emit dataChanged(index, section, le->text());
    emit dataCommitted(Complex, index, oldValue, newValue, section);

    if (section.compare(ComDataGui::Constants::TABLEROWFILTER) == 0)
        emit resetColumn(index);

    TableViewBase::commitData(editor);
}

/*!
 * \reimp
 */
void ComplexTableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    TableViewBase::closeEditor(editor, hint);
}

void ComplexTableView::init()
{
    setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onIndexDoubleClicked(QModelIndex)));
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SIGNAL(horizontalHeaderClicked(int)));
    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(columnResized(int,int,int)));
}

/*!
 * \brief 设置单元格数据.
 * \param column 列
 * \param verticalHeaderLabel 竖直标签名
 * \param data 数据
 */
void ComplexTableView::setCellData(int column, const QString &verticalHeaderLabel,
                                   const QVariant& data)
{
    int rowIndex = rowOfLabel(verticalHeaderLabel);
    QModelIndex mi = model()->index(rowIndex, column);
    QMap<int, QVariant> dataMap;
    dataMap.insert(Qt::DisplayRole, data);
    model()->setItemData(mi, dataMap);

    emit model()->layoutChanged();
}

/*!
 * \brief 获取列的行号.
 * \param verticalLabel 标签
 * \return 行号
 */
int ComplexTableView::rowOfLabel(const QString &verticalLabel)
{
    int rowCount = model()->rowCount();
    int rowIndex = -1;

    for (int i = 0; i < rowCount; ++i) {
        QString vhl = model()->headerData(i, Qt::Vertical, Qt::DisplayRole).toString();
        if (vhl.compare(verticalLabel) == 0) {
            rowIndex = i;
            break;
        }
    }
    return rowIndex;
}

void ComplexTableView::updatePreviewSize()
{

}

} // namespace ComDataGui

