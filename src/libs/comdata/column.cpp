#include "column.h"

#include <comutils/fileutils.h>

#include <QDataStream>
#include <QByteArray>

using namespace ComData;

/*!
 * \class ColumnBase
 * \brief 列表示数据的基类.
 *
 * \ingroup ComData
 *
 * 为列数据提供基础接口.
 */

//// ColumnBase
ColumnBase::ColumnBase(const ColumnBase &cb) : QObject(cb.parent())
{
    m_name = cb.m_name;
    m_longName = cb.m_longName;
    m_unit = cb.m_unit;
    m_comment = cb.m_comment;
    m_expression = cb.m_expression;
    m_significance = cb.m_significance;
    m_invalidCells = cb.m_invalidCells;
    m_markedCells = cb.m_markedCells;
}

QString ColumnBase::title() const
{
    switch (m_significance) {
    case X:
        return QString::fromLatin1("%1(X)").arg(m_name);
    case Y:
        return QString::fromLatin1("%1(Y)").arg(m_name);
    case Z:
        return QString::fromLatin1("%1(Z)").arg(m_name);
    case V:
        return QString::fromLatin1("%1(V)").arg(m_name);
    case VX:
        return QString::fromLatin1("%1(VX)").arg(m_name);
    case VY:
        return QString::fromLatin1("%1(VY)").arg(m_name);
    case VZ:
        return QString::fromLatin1("%1(VZ)").arg(m_name);
    case XError:
        return QString::fromLatin1("%1(XError)").arg(m_name);
    case YError:
        return QString::fromLatin1("%1(YError)").arg(m_name);
    case Label:
        return QString::fromLatin1("%1(Label)").arg(m_name);
    case Disregard:
        return QString::fromLatin1("%1(Disregard)").arg(m_name);
    }
}

/*!
 * \brief 返回列数据的JSON表示.
 */
QString ColumnBase::toJSON() const
{
    QStringList colJson;
    colJson << QString::fromLatin1("\"name\":\"%1\"").arg(CommonUtils::encodeForJSON(m_name));
    colJson << QString::fromLatin1("\"type\":%1").arg(type());
    colJson << QString::fromLatin1("\"size\":%1").arg(size());
    colJson << QString::fromLatin1("\"longName\":\"%1\"").arg(CommonUtils::encodeForJSON(m_longName));
    colJson << QString::fromLatin1("\"unit\":\"%1\"").arg(CommonUtils::encodeForJSON(m_unit));
    colJson << QString::fromLatin1("\"comment\":\"%1\"").arg(CommonUtils::encodeForJSON(m_comment));
    colJson << QString::fromLatin1("\"expression\":\"%1\"").arg(CommonUtils::encodeForJSON(m_expression));
    colJson << QString::fromLatin1("\"significance\":%1").arg(m_significance);

    colJson << QString::fromLatin1("\"value\":[%1]").arg(toJSONHelper());
    return QString::fromLatin1("{%1}").arg(colJson.join(QString::fromLatin1(",")));
}

/*!
 * \brief 序列化列数据到输出数据流 \a out 中.
 * \param out 输出数据流
 */
void ColumnBase::serialize(QDataStream &out) const
{
    out << quint8(type()) << qint32(size()) << m_name << m_longName << m_unit
        << m_comment << m_expression << quint8(m_significance);
    serializeHelper(out);
}

/*!
 * \brief 从数据输入流 \a in 中反序列化列数据.
 *
 * 在调用此方法之前, 列数据对象已经根据其类型和大小创建完毕了, 只需从输入流中获得列数据的其它属性.
 * \param in 输入数据流
 */
void ColumnBase::unserialize(QDataStream &in)
{
    quint8 sign;
    in >> m_name >> m_longName >> m_unit >> m_comment >> m_expression >> sign;
    m_significance = Significance(sign);
    unserializeHelper(in);
}

QVariant ColumnBase::extra(int role)
{
    return m_extraData.value(role);
}

void ColumnBase::setExtra(int role, const QVariant& data)
{
    m_extraData[role] = data;
}

/*!
 * \brief 根据列类型 \a ct, 与大小 \a size 创建列的具体对象.
 * \param ct 列类型
 * \param size 列的大小
 * \return 创建的列对象
 */
ColumnBase *ColumnBase::createColumn(ColumnType ct, int size, QObject *parent)
{
    ColumnBase* column = nullptr;
    switch (ct) {
    case Integer:
        column = new IntegerColumn(size, parent);
        break;
    case Float:
        column = new FloatColumn(size, parent);
        break;
    case Double:
        column = new DoubleColumn(size, parent);
        break;
    case FloatComplex:
        column = new FloatComplexColumn(size, parent);
        break;
    case DoubleComplex:
        column = new DoubleComplexColumn(size, parent);
        break;
    case String:
        column = new StringColumn(size, parent);
        break;
    case DateTime:
        column = new DateTimeColumn(size, parent);
        break;
    default:
        Q_ASSERT(false);
    }
    return column;
}

void ColumnBase::setInvalidCell(int index)
{
    if (!m_invalidCells.contains(index))
        m_invalidCells.append(index);
}

/*!
 * \brief 把无效的索引大于index的前移一位
 * \param index
 */
void ColumnBase::moveInvalidateCell(int index)
{
    for (int i = 0; i < m_invalidCells.size(); i++) {
        if (m_invalidCells[i] > index)
            m_invalidCells[i] -= 1;
    }
}

void ColumnBase::setMarkedIndex(int index)
{
    if (!m_markedCells.contains(index))
        m_markedCells.append(index);
}

/*!
 * \brief 把Marked的索引大于index的前移一位
 * \param index
 */
void ColumnBase::moveMarkedIndex(int index)
{
    for (int i = 0; i < m_markedCells.size(); i++) {
        if (m_markedCells[i] > index)
            m_markedCells[i] -= 1;
    }
}

QList<int> ColumnBase::invalidCells() const
{
    return m_invalidCells;
}

QList<int> ColumnBase::maskCells() const
{
    return m_markedCells;
}

//// IntegerColumn

/*!
 * \brief 把列中各个数据转换为JSON表示.
 * \return JSON表示的字符串
 */

void IntegerColumn::setCell(int i, const QVariant& var)
{
    setResizeCell(i);
    if (var != cell(i)) {
        setCellPrimitive(i, var.toInt());
        emit dataChanged();
    }
}

QString IntegerColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i)
        values << QString::number(m_cells[i]);
    return values.join(QString::fromLatin1(","));
}

void IntegerColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << *i;
}

void IntegerColumn::unserializeHelper(QDataStream &in)
{
    for (int i = 0; i < m_cells.size(); ++i)
        in >> m_cells[i];
}

void IntegerColumn::validateData()
{
    for (int i = 0; i < size(); ++i)
        setCell(i, qAbs(cell(i).toInt()));
}

//// FloatColumn

void FloatColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);

    if (cell(i) != var) {
        setCellPrimitive(i, var.toFloat());
        emit dataChanged();
    }
}

QString FloatColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i)
        values << QString::number(m_cells[i]);
    return values.join(QString::fromLatin1(","));
}

void FloatColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << *i;
}

void FloatColumn::unserializeHelper(QDataStream &in)
{
    for (int i = 0; i < m_cells.size(); ++i)
        in >> m_cells[i];
}

void FloatColumn::validateData()
{
    for (int i = 0; i < size(); ++i)
        setCell(i, qAbs(cell(i).toFloat()));
}

//// DoubleColumn

void DoubleColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);
    if (var.toDouble() != cell(i)) {
        setCellPrimitive(i, var.toDouble());
        emit dataChanged();
    }
}

QString DoubleColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i)
        values << QString::number(m_cells[i]);
    return values.join(QString::fromLatin1(","));
}

void DoubleColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << *i;
}

void DoubleColumn::unserializeHelper(QDataStream &in)
{
    for (int i = 0; i < m_cells.size(); ++i)
        in >> m_cells[i];
}

void DoubleColumn::validateData()
{
    for (int i = 0; i < size(); ++i)
        setCell(i, qAbs(cell(i).toDouble()));
}


//// FloatComplexColumn

QVariant FloatComplexColumn::cell(int i) const
{
    QVariant var;
    var.setValue(m_cells[i]);
    return var;
}

void FloatComplexColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);
    if (var != cell(i)) {
        std::complex<float> ic = var.value<std::complex<float>>();
        setCellPrimitive(i, ic);
        emit dataChanged();
    }
}

QString FloatComplexColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i) {
        values << QString::fromLatin1("[%1,%2]").arg(QString::number(m_cells[i].real()))
                  .arg(QString::number(m_cells[i].imag()));
    }
    return values.join(QString::fromLatin1(","));
}

void FloatComplexColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << (*i).real() << (*i).imag();
}

void FloatComplexColumn::unserializeHelper(QDataStream &in)
{
    float x, y;
    for (int i = 0; i < m_cells.size(); ++i) {
        in >> x >> y;
        m_cells[i].real(x);
        m_cells[i].imag(y);
    }
}

//// DoubleComplexColumn

QVariant DoubleComplexColumn::cell(int i) const
{
    QVariant var;
    var.setValue(m_cells[i]);
    return var;
}

void DoubleComplexColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);
    if (var != cell(i)) {
        std::complex<double> ic = var.value<std::complex<double>>();
        setCellPrimitive(i, ic);
        emit dataChanged();
    }
}

QString DoubleComplexColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i) {
        values << QString::fromLatin1("[%1,%2]").arg(QString::number(m_cells[i].real()))
                  .arg(QString::number(m_cells[i].imag()));
    }
    return values.join(QString::fromLatin1(","));
}

void DoubleComplexColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << (*i).real() << (*i).imag();
}

void DoubleComplexColumn::unserializeHelper(QDataStream &in)
{
    double x, y;
    for (int i = 0; i < m_cells.size(); ++i) {
        in >> x >> y;
        m_cells[i].real(x);
        m_cells[i].imag(y);
    }
}

//// StringColumn

void StringColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);
    if (var != cell(i)) {
        setCellPrimitive(i, var.toString());
        emit dataChanged();
    }
}

QString StringColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i) {
        values << QString::fromLatin1("\"%1\"").arg(CommonUtils::encodeForJSON(m_cells[i]));
    }
    return values.join(QString::fromLatin1(","));
}

void StringColumn::serializeHelper(QDataStream &out) const
{
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i)
        out << *i;
}

void StringColumn::unserializeHelper(QDataStream &in)
{
    for (int i = 0; i < m_cells.size(); ++i)
        in >> m_cells[i];
}

//// DateTimeColumn
void DateTimeColumn::setCell(int i, const QVariant &var)
{
    setResizeCell(i);

    // refactor point
    if (var != cell(i)) {
        setCellPrimitive(i, var.toDateTime());
        emit dataChanged();
    }
}

QString DateTimeColumn::toJSONHelper() const
{
    QStringList values;
    for (int i = 0; i < m_cells.size(); ++i) {
        values << QString::number(m_cells[i].toMSecsSinceEpoch());
    }
    return values.join(QString::fromLatin1(","));
}

void DateTimeColumn::serializeHelper(QDataStream &out) const
{
    qint64 msecs;
    for (auto i = m_cells.constBegin(); i != m_cells.constEnd(); ++i) {
        msecs = (*i).toMSecsSinceEpoch();
        out << msecs;
    }
}

void DateTimeColumn::unserializeHelper(QDataStream &in)
{
    qint64 time;
    for (int i = 0; i < m_cells.size(); ++i) {
        in >> time;
        m_cells[i].setMSecsSinceEpoch(time);
    }
}
