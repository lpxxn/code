#include "tablemodelbase.h"

#include <comdata/table.h>

namespace ComDataGui
{

TableModelBase::TableModelBase(QObject *parent) : QAbstractTableModel(parent)
{
}

/*!
 * \brief 设置横向标签.
 * \param labels 标签列表
 */
void TableModelBase::setHorizontalHeaderLabels(const QStringList& labels)
{
    m_horizontalHeaderLabels.clear();
    m_horizontalHeaderLabels = labels;
    emit headerDataChanged(Qt::Horizontal, 0, labels.size());
}

/*!
 * \brief 获取横向标签
 * \return 标签列表
 */
QStringList TableModelBase::horizontalHeaderLabels() const
{
    return m_horizontalHeaderLabels;
}

/*!
 * \brief 设置竖直标签.
 * \param labels 标签列表
 */
void TableModelBase::setVerticalHeaderLabels(const QStringList& labels)
{
    m_verticalHeaderLabels.clear();
    m_verticalHeaderLabels = labels;
    emit headerDataChanged(Qt::Vertical, 0, labels.size());
}

/*!
 * \brief 获取竖直标签列表.
 * \return 标签列表
 */
QStringList TableModelBase::verticalHeaderLabels() const
{
    return m_verticalHeaderLabels;
}

QModelIndex TableModelBase::createIndex(int row, int column)
{
    int dummy = 1;
    return QAbstractTableModel::createIndex(row, column, &dummy);
}

void TableModelBase::clear()
{

}

void TableModelBase::find(const QRegExp &exp)
{

}

void TableModelBase::setTable(ComData::Table* t)
{
    m_table = t;
    emit layoutChanged();
}

ComData::Table* TableModelBase::table() const
{
    return m_table;
}

} // namespace ComDataGui
