#ifndef COLUMN_H
#define COLUMN_H

#include "comdata_global.h"

#include <QString>
#include <QSet>
#include <QVariant>
#include <QDateTime>
#include <QVector>
#include <QDebug>

#include <complex>
#include <functional>
#include <algorithm>

namespace std
{
template <>
struct less<complex<float>>
    : public binary_function <complex<float>, complex<float>, bool>
{
    bool operator()(const complex<float>& c1, const complex<float>& c2) const
    {
        return abs(c1) < abs(c2);
    }
};

template <>
struct greater<complex<float>>
    : public binary_function <complex<float>, complex<float>, bool>
{
    bool operator()(const complex<float>& c1, const complex<float>& c2) const
    {
        return abs(c1) > abs(c2);
    }
};

template <>
struct less<complex<double>>
    : public binary_function <complex<double>, complex<double>, bool>
{
    bool operator()(const complex<double>& c1, const complex<double>& c2) const
    {
        return abs(c1) < abs(c2);
    }
};

template <>
struct greater<complex<double>>
    : public binary_function <complex<double>, complex<double>, bool>
{
    bool operator()(const complex<double>& c1, const complex<double>& c2) const
    {
        return abs(c1) > abs(c2);
    }
};

}

namespace ComData {

class COMDATA_EXPORT ColumnBase : public QObject
{
    Q_OBJECT
public:
    enum ColumnType {
        Integer,
        Float,
        Double,
        FloatComplex,
        DoubleComplex,
        String,
        DateTime,
    };

    enum Significance {
        X,
        Y,
        Z,
        V,
        VX,
        VY,
        VZ,
        XError,
        YError,
        Label,
        Disregard
    };

    ColumnBase(QObject* parent = nullptr) : QObject(parent) { m_significance = X;}
    ColumnBase(const ColumnBase& cb);
    virtual ~ColumnBase() {}
    virtual ColumnType type() const = 0;
    virtual int size() const = 0;
    virtual void resize(int size) = 0;
    virtual QVariant cell(int i) const = 0;
    virtual void setCell(int i, const QVariant& var) = 0;
    virtual void delCell(int begin, int end) = 0;
    virtual ColumnBase* clone() const = 0;
    virtual void sort(Qt::SortOrder so) = 0;
    virtual QVector<int> indiciesAfterSort(Qt::SortOrder so) = 0;
    virtual void reorder(const QVector<int>& indicies) = 0;
    virtual void clear() = 0;
    virtual bool isClean() const = 0;
    virtual void setClean(bool) = 0;

    QString title() const;
    virtual QString toJSON() const;
    void serialize(QDataStream& out) const;
    void unserialize(QDataStream &in);

    Significance significance() const { return m_significance; }
    void setSignificance(Significance s) { m_significance = s; validateData(); }
    QString name() const { return m_name;}
    void setName(const QString& name) { m_name = name; }
    QString longName() const { return m_longName; }
    void setLongName(const QString& longName) { m_longName = longName; }
    QString unit() const { return m_unit; }
    void setUnit(const QString& unit) { m_unit = unit; }
    QString comment() const { return m_comment; }
    void setComment(const QString& comment) { m_comment = comment; }
    QString expression() const { return m_expression; }
    void setExpression(const QString& expression) { m_expression = expression; }
    QVariant extra(int role);
    void setExtra(int role, const QVariant& data);

    static ColumnBase* createColumn(ColumnType ct, int size, QObject* parent = nullptr);

    void setMarkedIndex(int index);
    bool isMarked(int index) { return m_markedCells.contains(index); }
    void removeMarkedIndex(int index) { m_markedCells.removeAll(index); }
    void moveMarkedIndex(int index);
    void clearAllMarked() { m_markedCells.clear(); }

    bool isValid(int index) { return !m_invalidCells.contains(index); }
    void setInvalidCell(int index);
    void validateCell(int index) { m_invalidCells.removeAll(index); }
    void moveInvalidateCell(int index);
    void validateCells() { m_invalidCells.clear(); }
    QList<int> invalidCells() const;
    QList<int> maskCells() const;

signals:
    void dataChanged();

protected:
    virtual QString toJSONHelper() const = 0;
    virtual void serializeHelper(QDataStream& out) const = 0;
    virtual void unserializeHelper(QDataStream& in) = 0;
    virtual void validateData() = 0;

protected:
    QString             m_name;
    QString             m_longName;
    QString             m_unit;
    QString             m_comment;
    QString             m_expression;
    Significance        m_significance;
    QList<int>           m_invalidCells;
    QList<int>           m_markedCells;
    QMap<int, QVariant>  m_extraData;
};



template <typename T>
class Column : public ColumnBase
{
public:
    typedef T value_type;
    Column(QObject* parent = nullptr) : ColumnBase(parent) {}
    Column(const Column& c) : ColumnBase(c)
    {
        m_cells = c.m_cells;
    }

    Column(int size, QObject* parent = nullptr) : ColumnBase(parent)
    {
        Q_ASSERT(size >= 0);
        m_cells.reserve(size);
        for (int i = 0; i < size; ++i)
            m_cells.append(T());
    }

    virtual ~Column()
    {
        m_cells.clear();
    }

    virtual int size() const
    {
        return m_cells.size();
    }

    virtual void resize(int size)
    {
        m_cells.resize(size);
    }

    T at(int i) const
    {
        return m_cells[i];
    }

    void append(T cell)
    {
        m_cells.append(cell);
        m_isClean = false;
    }

    void insert(int i, T cell)
    {
        m_cells.insert(i, cell);
        m_isClean = false;
    }

    void delCell(int begin, int end)
    {
        typename QVector<T>::iterator beginIter = m_cells.begin() + begin;
        typename QVector<T>::iterator endIter = m_cells.begin() + end;
        m_cells.erase(beginIter, endIter);
        emit dataChanged();
    }

    const QVector<T>& data() const
    {
        return m_cells;
    }

    QVector<T>& data()
    {
        return m_cells;
    }

    void setResizeCell(int i)
    {
        if( i >= m_cells.size() || m_cells.size() == 0) {
            int collSize = m_cells.count();
            m_cells.resize(i + 1);
            for (collSize; collSize < i; collSize++) {
                setInvalidCell(collSize);
            }
        }
    }

    void setCellPrimitive(int i, T cell)
    {
        Q_ASSERT_X(i >= 0 && i < m_cells.size(), "Column<T>::setCellPrimitive", "index out of range");
        m_cells[i] = cell;
        m_isClean = false;
    }

    void setColumn(const QVector<T>& values)
    {
        m_cells = values;
        m_isClean = false;
    }

    void sort(Qt::SortOrder so)
    {
        clearAllMarked();
        const int n = m_invalidCells.count();
        if (n > 0) {
            QList<int> invalids;
            for (int i = 0; i < n; ++i) {
                invalids << i;
                if (m_invalidCells.contains(i)) {
                    m_invalidCells.removeAll(i);
                    continue;
                }
                int flag = -1;
                for (int k : m_invalidCells) {
                    if (k >= n) { // 交换无效的数据到前边
                        T temp = m_cells[i];
                        m_cells[i] = m_cells[k];
                        m_cells[k] = temp;
                        flag = k;
                        break;
                    }
                }
                if (flag != -1)
                    m_invalidCells.removeAll(flag);
            }
            m_invalidCells = invalids;
        }
        if (so == Qt::AscendingOrder)
            std::sort(m_cells.begin() + n, m_cells.end(), std::less<T>());
        else
            std::sort(m_cells.begin() + n, m_cells.end(), std::greater<T>());
        emit dataChanged();
    }

    /*!
     * \brief 按 \a so 排序列, 并返回列中排序后元素索引的数组.
     * \param so 排序方式
     */
    QVector<int> indiciesAfterSort(Qt::SortOrder so)
    {
        Q_ASSERT(m_invalidCells.isEmpty());
        QVector<int> indicies;
        const int sz = m_cells.size();
        indicies.reserve(sz);
        for (int i = 0; i < sz; ++i)
            indicies.push_back(i);

        if (so == Qt::AscendingOrder) {
            std::sort(indicies.begin(), indicies.end(),
                      [&](int i, int j) {return std::less<T>()(m_cells[i], m_cells[j]);});
            std::sort(m_cells.begin(), m_cells.end(), std::less<T>());
        } else {
            std::sort(indicies.begin(), indicies.end(),
                      [&](int i, int j) {return std::greater<T>()(m_cells[i], m_cells[j]);});
            std::sort(m_cells.begin(), m_cells.end(), std::greater<T>());
        }
        return indicies;
    }

    /*!
     * \brief 根据索引数组 \a indicies 重排列的元素.
     * \param indicies 索引数组
     */
    void reorder(const QVector<int>& indicies)
    {
        const int n = m_cells.size();
        if (n == 0)
            return;
        clearAllMarked();

        QVector<T> temp(n);
        QList<int> temp2;
        for (int i = 0; i < n; ++i) {
            const int i2 = indicies[i];
            if (m_invalidCells.contains(i2)) {
                m_invalidCells.removeAll(i2);
                temp2.append(i);
            }
            temp[i] = m_cells[i2];
        }
        m_cells = temp;
        m_invalidCells = temp2;

        emit dataChanged();
    }

    virtual void clear()
    {
        m_cells.clear();
        m_isClean = true;
    }

    virtual bool isClean() const
    {
        return m_isClean;
    }

    virtual void setClean(bool c)
    {
        m_isClean = c;
    }

protected:
    bool m_isClean = true;
    QVector<T> m_cells;
};

class COMDATA_EXPORT IntegerColumn : public Column<int>
{
public:
    IntegerColumn(QObject* parent = nullptr) : Column<int>(parent) {}
    IntegerColumn(int size, QObject* parent = nullptr) : Column<int>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new IntegerColumn(*this);
    }

    virtual ColumnType type() const { return Integer; }
    QVariant cell(int i) const { return m_cells[i]; }
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData();

};

class COMDATA_EXPORT FloatColumn : public Column<float>
{
public:
    FloatColumn(QObject* parent = nullptr) : Column<float>(parent) {}
    FloatColumn(int size, QObject* parent = nullptr) : Column<float>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new FloatColumn(*this);
    }

    virtual ColumnType type() const { return Float; }
    QVariant cell(int i) const { return m_cells[i]; }
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData();
};

class COMDATA_EXPORT DoubleColumn : public Column<double>
{
public:
    DoubleColumn(QObject* parent = nullptr) : Column<double>(parent) {}
    DoubleColumn(int size, QObject* parent = nullptr) : Column<double>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new DoubleColumn(*this);
    }

    virtual ColumnType type() const { return Double; }
    QVariant cell(int i) const { return m_cells[i]; }
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData();
};

class COMDATA_EXPORT FloatComplexColumn : public Column< std::complex<float> >
{
public:
    FloatComplexColumn(QObject* parent = nullptr) : Column<std::complex<float>>(parent) {}
    FloatComplexColumn(int size, QObject* parent = nullptr) : Column<std::complex<float>>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new FloatComplexColumn(*this);
    }

    virtual ColumnType type() const { return FloatComplex; }
    QVariant cell(int i) const;
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData() {}
};

class COMDATA_EXPORT DoubleComplexColumn : public Column< std::complex<double> >
{
public:
    DoubleComplexColumn(QObject* parent = nullptr) : Column<std::complex<double>>(parent) {}
    DoubleComplexColumn(int size, QObject* parent = nullptr) : Column<std::complex<double>>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new DoubleComplexColumn(*this);
    }

    virtual ColumnType type() const { return DoubleComplex; }
    QVariant cell(int i) const;
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData() {}
};

class COMDATA_EXPORT StringColumn : public Column<QString>
{
public:
    StringColumn(QObject* parent = nullptr) : Column<QString>(parent) {}
    StringColumn(int size, QObject* parent = nullptr) : Column<QString>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new StringColumn(*this);
    }

    virtual ColumnType type() const { return String; }
    QVariant cell(int i) const { return m_cells[i]; }
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData() {}
};

class COMDATA_EXPORT DateTimeColumn : public Column<QDateTime>
{
public:
    DateTimeColumn(QObject* parent = nullptr) : Column<QDateTime>(parent) {}
    DateTimeColumn(int size, QObject* parent = nullptr) : Column<QDateTime>(size, parent) {}

    virtual ColumnBase* clone() const
    {
        return new DateTimeColumn(*this);
    }

    virtual ColumnType type() const { return DateTime; }
    QVariant cell(int i) const { return m_cells[i]; }
    void setCell(int i, const QVariant& var);

protected:
    QString toJSONHelper() const;
    void serializeHelper(QDataStream& out) const;
    void unserializeHelper(QDataStream& in);
    void validateData() {}
};

}

Q_DECLARE_METATYPE(std::complex<float>)
Q_DECLARE_METATYPE(std::complex<double>)


#endif // COLUMN_H
