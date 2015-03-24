#ifndef TABLEMODELBASE_H
#define TABLEMODELBASE_H

#include <QAbstractTableModel>

namespace ComData
{
    class Table;
}

class QTableView;

namespace ComDataGui
{

class TableModelBase : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModelBase(QObject *parent = nullptr);

    void setHorizontalHeaderLabels(const QStringList& labels);
    QStringList horizontalHeaderLabels() const;

    void setVerticalHeaderLabels(const QStringList& labels);
    QStringList verticalHeaderLabels() const;

    QModelIndex createIndex(int row, int column);

    virtual void clear();
    virtual void find(const QRegExp& exp);

    void setTable(ComData::Table* t);
    ComData::Table* table() const;

protected:
    QStringList m_horizontalHeaderLabels;
    QStringList m_verticalHeaderLabels;
    ComData::Table* m_table = nullptr;
};

} // namespace ComDataGui

#endif // TABLEMODELBASE_H
