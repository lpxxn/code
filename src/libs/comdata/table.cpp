#include "table.h"

#include <QDebug>

namespace ComData {

/*!
 * \class Table
 * \brief 基于列组织的表数据.
 *
 * \ingroup ComData
 *
 * 一个表中可包括多个列, 表中列的类型可不同.
 */

class TablePrivate
{
public:
    QList<QSharedPointer<ColumnBase>> columns;
    QString name;
    static quint16 counter;
};

quint16 TablePrivate::counter = 0;

Table::Table(QObject *parent) : QObject(parent)
{
    d = new TablePrivate;
    ++TablePrivate::counter;
    d->name = tr("Table %1").arg(TablePrivate::counter);
}

Table::Table(const Table &table)
{
    d = new TablePrivate;
    ++TablePrivate::counter;
    d->name = tr("Table %1").arg(TablePrivate::counter);

    for (const QSharedPointer<ColumnBase>& cb : table.d->columns)
        d->columns << QSharedPointer<ColumnBase>(cb->clone());
}

Table::~Table()
{
    delete d;
    qDebug() << "Warning: Table destroyed!!!";
}

void Table::clear()
{
    d->columns.clear();
}

Table* Table::clone() const
{
    return new Table(*this);
}

/*!
 * \brief 序列化表中所有列数据到字节数组中.
 * \return 返回序列化后的字节数组
 */
QByteArray Table::serialize()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint32(d->columns.size());
    foreach (const QSharedPointer<ColumnBase>& cb, d->columns)
        cb->serialize(out);
    return data;
}

/*!
 * \brief 从 \a value 中反序列化数据到表中, 清除表中现有列的数据, 添加新的列数据.
 * \param value 包含表中数据的值
 */
void Table::unserialize(const QVariant &value)
{
    d->columns.clear();

    QByteArray data = value.toByteArray();
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_2);

    qint32 count = 0;
    in >> count;

    quint8 type;
    qint32 size;
    for (qint32 i = 0; i < count; ++i) {
        in >> type >> size;
        ColumnBase* col = ColumnBase::createColumn(ColumnBase::ColumnType(type), size);
        d->columns.append(QSharedPointer<ColumnBase>(col));
        col->unserialize(in);
    }
}

/*!
 * \brief 返回表中各列数据的JSON表示.
 */
QString Table::columnsJson() const
{
    QStringList colsJson;
    for (const QSharedPointer<ColumnBase>& cb : d->columns)
        colsJson << cb->toJSON();

    return QString::fromLatin1("[%1]").arg(colsJson.join(QString::fromLatin1(",")));
}

/*!
 * \brief 返回表中列指针对象的列表.
 */
QList<QSharedPointer<ColumnBase>> Table::columns() const
{
    return d->columns;
}

/*!
 * \brief 返回表中各列按序排列的标题字符串列表.
 */
QStringList Table::titles() const
{
    QStringList colTitles;
    for (const QSharedPointer<ColumnBase>& cb : d->columns)
        colTitles << cb->title();
    return colTitles;
}

/*!
 * \brief 根据当前表中各列的名称, 自动生成一个可用于表中新建列的名称.
 *
 * 列的名称从大写字母A开始排列, 如果超出了Z, 则使用两个大写字母如AA, AB等, 以此类推.
 * \return 可用于新建列的名称
 */
QString Table::autoColumnName() const
{
    QStringList names;
    for (const QSharedPointer<ColumnBase>& cb : d->columns)
        names << cb->name();

    QString baseName, name;
    while (true) {
        for (char c = 'A'; c <= 'Z'; ++c) {
            name = QString::fromLatin1("%1%2").arg(baseName).arg(c);
            if (!names.contains(name))
                goto END;
        }
        baseName += QStringLiteral("A");
    }
END:
    return name;
}

/*!
 * \brief 判断表中是否存在列的数据, 若有则返回true, 否则返回false.
 */
bool Table::isEmpty() const
{
    return d->columns.empty();
}

/*!
 * \brief 返回索引\a i 处的列对象.
 *
 * 若\a i 是无效索引则返回nullptr.
 * \param i 列的索引
 */
QSharedPointer<ColumnBase> Table::at(int i) const
{
    if (i < 0 || i >= d->columns.size())
        return QSharedPointer<ColumnBase>();
    return d->columns[i];
}

/*!
 * \brief 返回索引\a i 处的列对象.
 *
 * 若\a i 是无效索引则返回nullptr.
 * \param i 列的索引
 */
QSharedPointer<ColumnBase> Table::operator[](int i) const
{
    if (i < 0 || i >= d->columns.size())
        return QSharedPointer<ColumnBase>();
    return d->columns[i];
}

void Table::append(ColumnBase *columnData)
{
    QSharedPointer<ComData::ColumnBase> sp(columnData);
    d->columns.append(sp);
    connect(columnData, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

void Table::append(const QSharedPointer<ColumnBase> &column)
{
    d->columns.append(column);
    connect(column.data(), SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

/*!
 * \brief 向表中追加一列类型是 \a ct, 大小是 \a size 的列对象.
 * \param ct 列类型
 * \param size 列大小
 */
void Table::append(ColumnBase::ColumnType ct, int size)
{
    ColumnBase* columnData = ColumnBase::createColumn(ct, size);
    d->columns.append(QSharedPointer<ColumnBase>(columnData));
    connect(columnData, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

void Table::insert(int i, QSharedPointer<ColumnBase> column)
{
    if (column.isNull())
        return;

    d->columns.insert(i, column);
    connect(column.data(), SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

/*!
 * \brief 向表中插入一列类型是 \a ct, 大小是 \a size 的列对象.
 *
 * 若 \a i 小于或等于0, 将插入表的0位置, 若 \a i 大于或等于size(), 将追加在表的最后.
 * \param i 插入列的索引
 * \param ct 列类型
 * \param size 列大小
 */
void Table::insert(int i, ColumnBase::ColumnType ct, int size)
{
    ColumnBase* columnData = ColumnBase::createColumn(ct, size);
    d->columns.insert(i, QSharedPointer<ColumnBase>(columnData));
    connect(columnData, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}

/*!
 * \brief 从表中移除索引是 \a i 的列对象.
 *
 * 若 \a i 不在表列的有效索引范围内, 则此函数无动作. 在移除了列之后, 该列的数据将被删除.
 * \param i 列索引
 */
bool Table::removeAt(int i)
{
    if (i < 0 || i >= d->columns.size())
        return false;
//    ColumnBase* column = d->columns[i];
    d->columns.at(i).data()->blockSignals(true);
    d->columns[i].clear();
    d->columns.removeAt(i);
//    delete column;
    return true;
}

void Table::find(const QRegExp& exp)
{
    Q_UNUSED(exp)
}

void Table::replace(const QRegExp& oldExp, const QRegExp& newExp)
{
    Q_UNUSED(oldExp)
    Q_UNUSED(newExp)
}


int Table::rowCount() const
{
    if (d->columns.isEmpty())
        return 0;
    else
        d->columns.first()->size();
}

void Table::appendRow()
{
    for (int i = 0; i < d->columns.size(); ++i) {
        QSharedPointer<ComData::ColumnBase> colData = d->columns.at(i);
        colData->resize(colData->size() + 1);
    }
}

void Table::insertRow(int i)
{
    for (int colIndex = 0; colIndex < d->columns.size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = d->columns.at(colIndex);
        colData->resize(colData->size() + 1);
        ComData::ColumnBase* clone = colData->clone();
        colData->setCell(i, QVariant());
        for (int index = i + 1; index < colData->size(); ++index)
            colData->setCell(index, clone->cell(index - 1));
        delete clone;
    }
}

void Table::removeRow(int i)
{
    for (int colIndex = 0; colIndex < d->columns.size(); ++colIndex) {
        QSharedPointer<ComData::ColumnBase> colData = d->columns.at(colIndex);
        ComData::ColumnBase* clone = colData->clone();
        for (int dataIndex = i; dataIndex < (colData->size() - 1); ++dataIndex)
            colData->setCell(dataIndex, clone->cell(dataIndex + 1));
        colData->resize(colData->size() - 1);
        delete clone;
    }
}

void Table::setColumnType(int column, ColumnBase::ColumnType type)
{
    ColumnBase* col;
    switch (type) {
    case ColumnBase::Integer:
        col = new IntegerColumn;
        break;

    case ColumnBase::Float:
        col = new FloatColumn;
        break;

    case ColumnBase::Double:
        col = new DoubleColumn;
        break;

    case ColumnBase::FloatComplex:
        col = new FloatComplexColumn;
        break;

    case ColumnBase::DoubleComplex:
        col = new DoubleComplexColumn;
        break;

    case ColumnBase::String:
        col = new StringColumn;
        break;

    case ColumnBase::DateTime:
        col = new DateTimeColumn;
        break;
    }

    ColumnBase* oldCol = d->columns[column].data();
    col->resize(oldCol->size());
    col->setSignificance(oldCol->significance());
    col->setName(oldCol->name());
    col->setLongName(oldCol->longName());
    col->setComment(oldCol->comment());
    col->setUnit(oldCol->unit());
    col->setExpression(oldCol->expression());

    for (int i = 0; i < oldCol->size(); ++i)
        col->setCell(i, oldCol->cell(i));

    d->columns[column].reset(col);
}

/*!
 * \brief 返回表中列的数量.
 */
int Table::size() const
{
    return d->columns.size();
}

/*!
 * \brief 交换表中的两列.
 * \param i 列索引
 * \param j 列索引
 * \return 成功则返回true, 否则返回false
 */
bool Table::swap(int i, int j)
{
    if (i < 0 || i >= d->columns.size() || j < 0 || j >= d->columns.size())
        return false;
    d->columns.swap(i, j);
    return true;
}

/*!
 * \brief 把列从i移动到j
 * \param i
 * \param j
 * \return
 */
bool Table::moveTo(int i, int j)
{
    if (i < 0 || i >= d->columns.size() || j < 0 || j >= d->columns.size())
        return false;
    d->columns.move(i, j);
    return true;
}

/*!
 * \brief 基于第 \a index 列对整个表中各列按 \a so 排序.
 *
 * \param index 表中列的索引
 * \param so 排序方式
 * \return 如果第 index 列含有无效的数据, 或者表中各列的长度不等则不排序, 返回false, 否则排序并返回true.
 * \note 基于含有无效数据列对整个表排序无意义.
 */
bool Table::sortByColumn(int index, Qt::SortOrder so)
{
    QSharedPointer<ColumnBase> column = d->columns[index];
    if (!column->invalidCells().isEmpty())
        return false;

    const int n = column->size();
    const int colNum = d->columns.size();
    for (int i = 0; i < colNum; ++i) {
        int sz = d->columns[i]->size();
        if (sz && sz != n)
            return false;
    }

    QVector<int> indicies = column->indiciesAfterSort(so);
    for (int i = 0; i < colNum; ++i)
        if (i != index)
            d->columns[i]->reorder(indicies);
    return true;
}

void Table::onDataChanged()
{
    ComData::ColumnBase* columnData = qobject_cast<ComData::ColumnBase*>(sender());
    for (int i = 0; i < d->columns.size(); ++i) {
        if (columnData == d->columns.at(i).data()) {
            emit columnDataChanged(i, columnData);
            break;
        }
    }
}

}
