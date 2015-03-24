#ifndef COMPLEXMODEL_H
#define COMPLEXMODEL_H

#include "tablemodelbase.h"

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QMap>

class QAbstractItemView;
class QTableView;
class QSpinBox;
class QSettings;

namespace ComDataGui
{

class ComplexModel : public TableModelBase
{
    Q_OBJECT
public:
    explicit ComplexModel(QObject *parent = nullptr);
    ComplexModel(const ComplexModel &complexModel);

public:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                       int role = Qt::EditRole);

    void setBackgroundColor(const QColor& clr) { m_backgroundColor = clr; }
    QColor backgroundColor() const {return m_backgroundColor;}

    bool isVerticalLabelVisible() const;
    void setVerticalLabelVisible(bool visible);

    void applySettings();
    void initVerticalLabels();

    int verticalLabelIndex(const QString& label) const;

signals:
    void updateIndex(const QModelIndex& index, int type);
    void rowVisible(const QString &rowName, bool shown);

private:
    void setRowVisible(const QSettings *setting, const QString &settingStr, const QString &rowName, bool isvisibility = true);
    void insertTitle(const QString &rowName, int index);

private:
    bool m_verticalLabelVisible;
    QColor m_backgroundColor;    
};

} // namespace ComDataGui

#endif // COMPLEXMODEL_H
