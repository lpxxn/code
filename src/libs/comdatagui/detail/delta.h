#ifndef DELTA_H
#define DELTA_H

#include <QString>
#include <QObject>
#include <QAbstractItemModel>

#include <comdata/column.h>
#include <comdata/table.h>

#include "../columnpropertydata.h"
#include "complextableview.h"

namespace ComDataGui
{

class DeltaBase
{
public:
    enum Type
    {
        Data,
        Complex,
        RowVisibility,
        ElementVisibility,
        ColumnVisibility,
        SetSignificance, // one column
        SetSignificances, // multiple columns
        MarkCell,
        DeleteColumn,
        InsertColumn,
        AppendColumn,
        CustomFunction,
        Filter,
        InsertRow,
        AppendRow,
        ColumnPropChanged,
        ClearTable,
        SwapColumn,
        MoveColumn,
        Count
    };

    virtual ~DeltaBase() {}
    virtual int type() const = 0;

    ComData::Table* table = nullptr;
};

typedef QList<DeltaBase*> DeltaDataList;

class DeltaData : public DeltaBase
{
public:
    virtual int type() const { return Data; }
    static DeltaData* create(ComData::Table* table, int c, int r, const QVariant& oldVal, const QVariant& newVal)
    {
        DeltaData* dd = new DeltaData;
        dd->table = table;
        dd->column = c;
        dd->row = r;
        dd->oldValue = oldVal;
        dd->newValue = newVal;
        return dd;
    }

    int column;
    int row;
    QVariant oldValue;
    QVariant newValue;
};

class DeltaComplex : public DeltaBase
{
public:
    virtual int type() const { return Complex; }
    static DeltaComplex* create(ComData::Table* table, const QString& s, int c, int r, const QVariant& oldVal, const QVariant& newVal)
    {
        DeltaComplex* dc = new DeltaComplex;
        dc->table = table;
        dc->section = s;
        dc->column = c;
        dc->row = r;
        dc->oldValue = oldVal;
        dc->newValue = newVal;
        return dc;
    }

    QString section;
    int column;
    int row;
    QVariant oldValue;
    QVariant newValue;
};

class DeltaRowVisibility : public DeltaBase
{
public:
    virtual int type() const { return RowVisibility; }
    static DeltaRowVisibility* create(bool oldV, bool newV, const QString& e)
    {
        DeltaRowVisibility* dv = new DeltaRowVisibility;
        dv->oldVisible = oldV;
        dv->newVisible = newV;
        dv->rowName = e;
        return dv;
    }

    bool oldVisible;
    bool newVisible;
    QString rowName;
};

class DeltaColumnVisibility : public DeltaBase
{
public:
    virtual int type() const { return ColumnVisibility; }
    static DeltaColumnVisibility* create(const QList<int>& columns, bool oldV, bool newV)
    {
        DeltaColumnVisibility* dcv = new DeltaColumnVisibility;
        dcv->columns = columns;
        dcv->oldVisible = oldV;
        dcv->newVisible = newV;
        return dcv;
    }

    QList<int> columns;
    bool oldVisible;
    bool newVisible;
};

class DeltaElementVisibility : public DeltaBase
{
public:
    virtual int type() const { return ElementVisibility; }
    static DeltaElementVisibility* create(bool oldV, bool newV, const QString& e)
    {
        DeltaElementVisibility* dv = new DeltaElementVisibility;
        dv->oldVisible = oldV;
        dv->newVisible = newV;
        dv->element = e;
        return dv;
    }

    bool oldVisible;
    bool newVisible;
    QString element;
};

class DeltaSetSignificance : public DeltaBase
{
public:
    virtual int type() const { return SetSignificance; }
    static DeltaSetSignificance* create(int column, ComData::ColumnBase::Significance oldSig,
                                        ComData::ColumnBase::Significance newSig)
    {
        DeltaSetSignificance* dss = new DeltaSetSignificance;
        dss->column = column;
        dss->oldSig = oldSig;
        dss->newSig = newSig;
        return dss;
    }

    int column;
    ComData::ColumnBase::Significance oldSig;
    ComData::ColumnBase::Significance newSig;
};

class DeltaSetSignificances : public DeltaBase
{
public:
    virtual int type() const { return SetSignificances; }
    static DeltaSetSignificances* create(const QMap<int, QPair<ComData::ColumnBase::Significance, ComData::ColumnBase::Significance> > columnSigs)
    {
        DeltaSetSignificances* dss = new DeltaSetSignificances;
        dss->columnSigs = columnSigs;
        return dss;
    }

    QMap<int, QPair<ComData::ColumnBase::Significance, ComData::ColumnBase::Significance> > columnSigs;
};

class DeltaMarkCell : public DeltaBase
{
public:
    virtual int type() const { return MarkCell; }
    static DeltaMarkCell* create(const QModelIndexList& list, bool oldM, bool newM)
    {
        DeltaMarkCell* dmc = new DeltaMarkCell;
        dmc->indexList = list;
        dmc->oldMark = oldM;
        dmc->newMark = newM;
        return dmc;
    }

    QModelIndexList indexList;
    bool oldMark;
    bool newMark;
};

class DeltaDeleteColumn : public DeltaBase
{
public:
    ~DeltaDeleteColumn()
    {
        QList<ComData::ColumnBase*> columnDataList = columnDataMap.values();
        qDeleteAll(columnDataList);
    }

    virtual int type() const { return DeleteColumn; }
    static DeltaDeleteColumn* create(const QMap<int, ComData::ColumnBase*>& columnDataMap)
    {
        DeltaDeleteColumn* ddc = new DeltaDeleteColumn;
        ddc->columnDataMap = columnDataMap;
        return ddc;
    }

    QMap<int, ComData::ColumnBase*> columnDataMap;
};

class DeltaInsertColumn : public DeltaBase
{
public:
    ~DeltaInsertColumn()
    {
        delete columnData;
    }

    virtual int type() const { return InsertColumn; }
    static DeltaInsertColumn* create(int column, ComData::ColumnBase* columnData)
    {
        DeltaInsertColumn* dic = new DeltaInsertColumn;
        dic->column = column;
        dic->columnData = columnData->clone();
        return dic;
    }

    int column;
    ComData::ColumnBase* columnData;
};

class DeltaAppendColumn : public DeltaBase
{
public:
    ~DeltaAppendColumn()
    {
        delete columnData;
    }

    virtual int type() const { return AppendColumn; }
    static DeltaAppendColumn* create(ComData::ColumnBase* columnData)
    {
        DeltaAppendColumn* dac = new DeltaAppendColumn;
        dac->columnData = columnData->clone();
        return dac;
    }

    ComData::ColumnBase* columnData;
};

class DeltaCustomFunction : public DeltaBase
{
public:
    ~DeltaCustomFunction()
    {
        qDeleteAll(dataList);
        dataList.clear();
    }

    virtual int type() const { return CustomFunction; }
    static DeltaCustomFunction* create(ComData::ColumnBase* colData, ComData::Table* table,
                                       const DeltaDataList& dataList, const QString& oldFunc,
                                       const QString& newFunc)
    {
        DeltaCustomFunction* dcf = new DeltaCustomFunction;
        dcf->columnData = colData;
        dcf->table = table;
        dcf->dataList = dataList;
        dcf->oldFunction = oldFunc;
        dcf->newFunction = newFunc;
        return dcf;
    }

    ComData::ColumnBase* columnData = nullptr;
    QString oldFunction;
    QString newFunction;
    DeltaDataList dataList;
};

class DeltaFilter : public DeltaBase
{
public:
    ~DeltaFilter()
    {
        qDeleteAll(dataList);
        dataList.clear();
    }

    virtual int type() const { return Filter; }
    static DeltaFilter* create(const QModelIndex& index, const FilterData& oldF,
                               const FilterData& newF, ComData::Table* table,
                               const DeltaDataList& dataList)
    {
        DeltaFilter* df = new DeltaFilter;
        df->index = index;
        df->oldFilter = oldF;
        df->newFilter = newF;
        df->table = table;
        df->dataList = dataList;
        return df;
    }

    QModelIndex index;
    FilterData oldFilter;
    FilterData newFilter;
    DeltaDataList dataList;
};

class DeltaInsertRow : public DeltaBase
{
public:
    virtual int type() const { return InsertRow; }
    static DeltaInsertRow* create(ComData::Table* table, int row)
    {
        DeltaInsertRow* dir = new DeltaInsertRow;
        dir->table = table;
        dir->row = row;
        return dir;
    }

    int row;
};

class DeltaAppendRow : public DeltaBase
{
public:
    virtual int type() const { return AppendRow; }

    static DeltaAppendRow* create(ComData::Table* table)
    {
        DeltaAppendRow* dar = new DeltaAppendRow;
        dar->table = table;
        return dar;
    }
};

class DeltaColumnPropChanged : public DeltaBase
{
public:
    virtual int type() const { return ColumnPropChanged; }

    static DeltaColumnPropChanged* create(const ComDataGui::ColumnPropertyData& oldProp, const ComDataGui::ColumnPropertyData& newProp)
    {
        DeltaColumnPropChanged* dcpc = new DeltaColumnPropChanged;
        dcpc->oldProp = oldProp;
        dcpc->newProp = newProp;
        return dcpc;
    }

    ComDataGui::ColumnPropertyData oldProp;
    ComDataGui::ColumnPropertyData newProp;
};

class DeltaClearTable : public DeltaBase
{
public:
    ~DeltaClearTable()
    {
        delete oldTable;
    }

    virtual int type() const { return ClearTable; }

    static DeltaClearTable* create(ComData::Table* oldTable, ComData::Table* newTable)
    {
        DeltaClearTable* dct = new DeltaClearTable;
        dct->oldTable = oldTable;
        dct->newTable = newTable;
        return dct;
    }

    ComData::Table* oldTable;
    ComData::Table* newTable;
};

class DeltaSwapColumn : public DeltaBase
{
public:
    virtual int type() const { return SwapColumn; }

    static DeltaSwapColumn* create(int from, int to)
    {
        DeltaSwapColumn* dsc = new DeltaSwapColumn;
        dsc->from = from;
        dsc->to = to;
        return dsc;
    }

    int from;
    int to;
};

class DeltaMoveColumn : public DeltaSwapColumn
{
public:
    virtual int type() const { return MoveColumn; }

    static DeltaMoveColumn* create(int from, int to)
    {
        DeltaMoveColumn* dmc = new DeltaMoveColumn;
        dmc->from = from;
        dmc->to = to;
        return dmc;
    }
};

}
#endif // DELTA_H
