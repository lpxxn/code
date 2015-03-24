#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "tablemodelbase.h"

#include <comdata/column.h>
#include <QFont>
#include <QColor>

class QTableView;

namespace ComData
{
    class Table;
}

namespace ComDataGui
{
class DataModelStyle;

class DataModel : public TableModelBase
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    QSharedPointer<ComData::ColumnBase> column(int index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addDataColumn(ComData::ColumnBase::ColumnType ct);
    void addColumn(ComData::ColumnBase* columnData);
    void addRow();
    void removeColumn(int column);
    void setMarkedCell(const QModelIndex &index, bool marked);
    void setColumnType(int column, ComData::ColumnBase::ColumnType type);

    void sortColumn(const QModelIndex &mi, Qt::SortOrder order);
    void sortColumn(int index, Qt::SortOrder order);
    void sortTableByColumn(int column, Qt::SortOrder order);
    void setRowCount(int count);
    void setFont(const QFont& font);
    void setFontColor(const QColor& clr);
    void setBackgroundColor(const QColor& clr);
    void setDisplayFormat(char format);

    void setColumnFormat(int column, char format);
    char columnFormat(int column);

    void setPrecision(int precision);
    int precision() const;

    void setColumnPrecision(int column, int precision);
    int columnPrecision(int column);

    void applySetting();
    DataModelStyle* modelStyle() const {return m_modelStyle;}

    virtual void clear();
    virtual void setTable(ComData::Table *t);

public slots:
    void setSignificance(int index, ComData::ColumnBase::Significance sign);

signals:
    void columnDataChanged(int column, const ComData::ColumnBase* columnData);

private slots:
    void onColumnDataChanged(int column, const ComData::ColumnBase* columnData);

private:
    bool haveMaskedValue();
    bool isValid(const QModelIndex& index) const;   

private:
    DataModelStyle* m_modelStyle = nullptr;
};

} // namespace ComDataGui


#endif // DATAMODEL_H
