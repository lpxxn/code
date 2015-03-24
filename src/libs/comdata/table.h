#ifndef TABLE_H
#define TABLE_H

#include "column.h"

#include <QObject>
#include <QStringList>
#include <QSharedPointer>

namespace ComData {

class COMDATA_EXPORT Table : public QObject
{
    Q_OBJECT
public:
    Table(QObject* parent = nullptr);
    Table(const Table& table);
    ~Table();

    void clear();

    Table* clone() const;

    QByteArray serialize();
    void unserialize(const QVariant &value);

    QString columnsJson() const;
    QList<QSharedPointer<ColumnBase>> columns() const;
    QStringList titles() const;
    QString autoColumnName() const;
    bool isEmpty() const;

    QSharedPointer<ColumnBase> at(int i) const;
    QSharedPointer<ColumnBase> operator[](int i) const;

    void append(ComData::ColumnBase* columnData);
    void append(const QSharedPointer<ColumnBase>& column);
    void append(ColumnBase::ColumnType ct, int size = 0);
    void insert(int i, QSharedPointer<ColumnBase> column);
    void insert(int i, ColumnBase::ColumnType ct, int size = 0);
    bool removeAt(int i);

    void find(const QRegExp& exp);
    void replace(const QRegExp& oldExp, const QRegExp& newExp);

    int rowCount() const;
    void appendRow();
    void insertRow(int i);
    void removeRow(int i);

    void setColumnType(int column, ColumnBase::ColumnType type);

    int size() const;

    template <typename Column>
    bool fillColumnAt(int i, const QVector<typename Column::value_type>& values)
    {
        QSharedPointer<Column> col = at(i).dynamicCast<Column>();
        if (col) {
            col->setColumn(values);
            return true;
        }
        return false;
    }

    template <typename Column>
    bool appendRowAt(int col, typename Column::value_type value)
    {
        QSharedPointer<Column> column = at(col).dynamicCast<Column>();
        if (column) {
            column->append(value);
            return true;
        }
        return false;
    }

    template <typename Column>
    bool insertRowAt(int col, int row, typename Column::value_type value)
    {
        QSharedPointer<Column> column = at(col).dynamicCast<Column>();
        if (column) {
            if (row < 0 || row > column->size())
                return false;
            column->insert(row, value);
            return true;
        }
        return false;
    }

    template <typename Column>
    QVector<typename Column::value_type> columnAt(int i)
    {
        QSharedPointer<Column> col = at(i).dynamicCast<Column>();
        return col ? col->data() : QVector<typename Column::value_type>();
    }

    bool swap(int i, int j);
    bool moveTo(int i, int j);
    bool sortByColumn(int index, Qt::SortOrder so);

signals:
    void columnDataChanged(int column, const ComData::ColumnBase* columnData);

private slots:
    void onDataChanged();

private:
    class TablePrivate* d;
};

}


#endif // TABLE_H
