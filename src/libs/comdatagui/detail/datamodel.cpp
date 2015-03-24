#include "datamodel.h"

#include <comutils/settingsmanager.h>
#include <comdata/table.h>

#include <QDebug>
#include <QBrush>
#include <QSettings>

using namespace ComData;

namespace ComDataGui
{

class DataModelStyle
{
public:
    DataModelStyle()
    {
        initMaskColors();
    }

    DataModelStyle(const DataModelStyle& style) : DataModelStyle()
    {
        modelRowCount = style.modelRowCount;
        font = style.font;
        fontColor = style.fontColor;
        backgroundColor = style.backgroundColor;
        format = style.format;
        precision = style.precision;
        enableMask = style.enableMask;
        currentMskColor = style.currentMskColor;
    }

    void initMaskColors()
    {
        maskColors << (int)Qt::blue
                   << (int)Qt::red
                   << (int)Qt::black
                   << (int)Qt::green
                   << (int)Qt::yellow
                   << (int)Qt::gray
                   << (int)Qt::cyan
                   << (int)Qt::gray;
        currentMskColor = QPair<int,int>(0, maskColors.first());
    }

    void applySetting()
    {
        CommonUtils::SettingsManager* sm = CommonUtils::SettingsManager::instance();
        QSettings *setting = sm->generalSettings();

        font = setting->value(QStringLiteral("Table/Font"), QFont()).value<QFont>();
        fontColor = setting->value(QStringLiteral("Table/FontColor"), QColor(QString("black"))).value<QColor>();
        backgroundColor = setting->value(QStringLiteral("Table/DataBackColor"), QColor(Qt::transparent)).value<QColor>();
        modelRowCount = setting->value(QStringLiteral("Table/RowNumber"), 10).toInt();
        format =  setting->value(QStringLiteral("Table/Display"), 'g').toChar().toLatin1();
        precision = setting->value(QStringLiteral("Table/DecimalPlace"), 5).toInt();

    }

    void setColumnPrecision(int column, int precision)
    {
        columnPropMap[column].precision = precision;
    }

    int columnPrecision(int column)
    {
        return columnPropMap.value(column).precision;
    }

    void setColumnFormat(int column, char format)
    {
        columnPropMap[column].format = format;
    }

    char columnFormat(int column)
    {
        return columnPropMap.value(column).format;
    }

    int modelRowCount = 10;
    QFont font;
    QColor fontColor;
    QColor backgroundColor;
    char format;
    int precision;
    bool enableMask = true;
    QList<int> maskColors;
    QPair<int,int> currentMskColor;

    struct ColumnProperty
    {
        char format;
        int precision;
    };
    QMap<int, ColumnProperty> columnPropMap;
};

DataModel::DataModel(QObject *parent) :
    TableModelBase(parent)
{
    m_modelStyle = new DataModelStyle();
}

DataModel::~DataModel()
{
    delete m_modelStyle;
    m_modelStyle = nullptr;
}

QSharedPointer<ColumnBase> DataModel::column(int index) const
{
    Q_ASSERT(index < m_table->size());
    return m_table->at(index);
}

/*!
 * \reimp
 */
int DataModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    int rowMax = 0;

    if (m_table)
        for (int i = 0, len = m_table->size(); i < len; i++)
            rowMax = std::max(rowMax, m_table->at(i)->size());

    return std::max(rowMax, m_modelStyle->modelRowCount);
}

/*!
 * \reimp
 */
int DataModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    if (m_table)
        return m_table->size();
    else
        return 0;
}

bool DataModel::isValid(const QModelIndex &index) const
{
    if (!index.isValid() || !m_table)
        return false;

    const int row = index.row();
    const int col = index.column();
    const int colSize = m_table->size();
    if (colSize == 0)
        return false;

    return (col >= 0 && col < colSize) && (row >= 0 && row < m_table->at(col)->size());
}

/*!
 * \brief 处理要显示的数据.
 * \param table 数据表
 * \param row 行索引
 * \param col 列索引
 * \param style 数据模型风格
 * \param ok 是否处理成功
 * \return 处理后的数据
 */
static QVariant processTableData(ComData::Table* table, int row, int col, DataModelStyle* style, bool& ok)
{
    const int colSize = table->size();
    if (colSize == 0) {
        return QVariant();
    }

    if (!((col >= 0 && col < colSize) && (row >= 0 && row < table->at(col)->size()))) {
        return QVariant();
    }

    ok = true;
    QSharedPointer<ColumnBase> column = table->at(col);
    if (!column->isValid(row))
        return QStringLiteral("--");

    if (column->cell(row) == QVariant::Invalid)
        return QString();

    switch (column->type()) {
    case ComData::ColumnBase::ColumnType::Double:
    case ComData::ColumnBase::ColumnType::Float:
    {
        if (column->isClean())
            return QVariant();

        char format = style->format;
        int precision = style->precision;
        if (style->columnPropMap.contains(col))
            format = style->columnFormat(col);
        if (style->columnPropMap.contains(col))
            precision = style->columnPrecision(col);

        QString str;
        if (format == 'f')
            str = QString("%L1").arg(column->cell(row).toDouble(), 0, format, precision);
        else
            str = QString::number(column->cell(row).toDouble(), format, precision);

        return str;
    }
        break;

        // handling complex number display
    case ComData::ColumnBase::ColumnType::DoubleComplex:
    {
        std::complex<double> cn = qvariant_cast<std::complex<double> >(column->cell(row));
        return QString("%1+%2i").arg(cn.real()).arg(cn.imag());
    }

    case ComData::ColumnBase::ColumnType::FloatComplex:
    {
        std::complex<float> cn = qvariant_cast<std::complex<float> >(column->cell(row));
        return QString("%1+%2i").arg(cn.real()).arg(cn.imag());
    }

    default:
        break;
    }
    return column->cell(row);
}
template <int role>
class RoleHandler;

template <>
class RoleHandler<Qt::DisplayRole>
{
public:
    QVariant operator()(ComData::Table* table, int row, int col, DataModelStyle* style, bool& ok, int role)
    {
        if (role != Qt::DisplayRole) {
            ok = false;
            return QVariant();
        }

        return processTableData(table, row, col, style, ok);
    }
};

template <>
class RoleHandler<Qt::EditRole>
{
public:
    QVariant operator()(ComData::Table* table, int row, int col, DataModelStyle* style, bool& ok, int role)
    {
        if (role != Qt::EditRole) {
            ok = false;
            return QVariant();
        }

        return processTableData(table, row, col, style, ok);
    }
};

template <>
class RoleHandler<Qt::ForegroundRole>
{
public:
    QVariant operator()(ComData::Table* table, int row, int col, DataModelStyle* style, bool& ok, int role)
    {
        if (role != Qt::ForegroundRole) {
            ok = false;
            return QVariant();
        }

        ok = true;
        QSharedPointer<ColumnBase> column = table->at(col);
        if (column->isMarked(row) && style->enableMask)
            return qVariantFromValue(QBrush((Qt::GlobalColor)(style->currentMskColor.second)));
        return QBrush(style->fontColor);
    }
};

template <>
class RoleHandler<Qt::FontRole>
{
public:
    QVariant operator()(ComData::Table* table, int row, int col, DataModelStyle* style, bool& ok, int role)
    {
        if (role != Qt::FontRole) {
            ok = false;
            return QVariant();
        }

        ok = true;
        QSharedPointer<ColumnBase> column = table->at(col);
        if (!column->isValid(row))
            return QFont();
        return style->font;
    }
};

template <>
class RoleHandler<Qt::BackgroundRole>
{
public:
    QVariant operator()(DataModelStyle* style, bool& ok, int role)
    {
        if (role != Qt::BackgroundRole) {
            ok = false;
            return QVariant();
        }

        ok = true;
        return QBrush(style->backgroundColor);
    }
};

/*!
 * \reimp
 */
QVariant DataModel::data(const QModelIndex & index, int role) const
{
    const int row = index.row();
    const int col = index.column();

    bool ok = false;

    QVariant v;
    v = RoleHandler<Qt::DisplayRole>()(m_table, row, col, m_modelStyle, ok, role);

    if (!ok)
        v = RoleHandler<Qt::EditRole>()(m_table, row, col, m_modelStyle, ok, role);

    if (!ok)
        v = RoleHandler<Qt::ForegroundRole>()(m_table, row, col, m_modelStyle, ok, role);

    if (!ok)
        v = RoleHandler<Qt::FontRole>()(m_table, row, col, m_modelStyle, ok, role);

    if (!ok)
        v = RoleHandler<Qt::BackgroundRole>()(m_modelStyle, ok, role);

    return v;
}

/*!
 * \reimp
 */
Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

/*!
 * \reimp
 */
bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    if (!index.isValid())
        return false;

    QSharedPointer<ColumnBase> cb = m_table->at(index.column());
    if (!cb->isValid(index.row()))
        cb->validateCell(index.row());

    cb->setCell(index.row(), value);
    return true;
}

/*!
 * \brief 增加制定类型的列.
 * \param ct 列类型
 */
void DataModel::addDataColumn(ColumnBase::ColumnType ct)
{
    m_table->append(ct);
}

/*!
 * \brief 增加列数据.
 * \param columnData 列数据对象
 */
void DataModel::addColumn(ColumnBase *columnData)
{
    m_table->append(QSharedPointer<ColumnBase>(columnData));
}

/*!
 * \brief 增加行.
 */
void DataModel::addRow()
{
    m_modelStyle->modelRowCount++;
}

/*!
 * \brief 删除列.
 * \param column 列索引
 */
void DataModel::removeColumn(int column)
{
    QSharedPointer<ComData::ColumnBase> colData = m_table->at(column);
    if (!colData.isNull()) {
        colData->clearAllMarked();
        colData->validateCells();
        m_table->removeAt(column);
        emit layoutChanged();
    }
}

/*!
 * \brief 标记制定索引的单元格.
 * \param index 单元格索引
 * \param marked 是否标记
 */
void DataModel::setMarkedCell(const QModelIndex &index, bool marked)
{
    QSharedPointer<ColumnBase> column = m_table->at(index.column());
    marked ? column->setMarkedIndex(index.row()) : column->removeMarkedIndex(index.row());
}

/*!
 * \brief 设置列的数据类型.
 * \param column 列索引
 * \param type 类型
 */
void DataModel::setColumnType(int column, ColumnBase::ColumnType type)
{
    if (this->column(column)->type() == type)
        return;

    m_table->setColumnType(column, type);
}

/*!
 * \brief 设置列的意义.
 * \param index 列索引
 * \param sign 意义
 */
void DataModel::setSignificance(int index, ComData::ColumnBase::Significance sign)
{
    m_table->at(index)->setSignificance(sign);
}

void DataModel::onColumnDataChanged(int column, const ColumnBase *columnData)
{
    Q_UNUSED(columnData)
    Q_UNUSED(column)
}

bool DataModel::haveMaskedValue()
{
    for (int i = 0; i < m_table->size(); i++) {
        if (m_table->at(i)->maskCells().count() > 0)
            return true;
    }
    return false;
}

/*!
 * \brief 列排序
 * \param mi 单元格索引
 * \param order 排列顺序
 */
void DataModel::sortColumn(const QModelIndex& mi, Qt::SortOrder order)
{
    sortColumn(mi.column(), order);
}

// todo 基类有虚函数sort，覆写它
void DataModel::sortColumn(int index, Qt::SortOrder order)
{
    QSharedPointer<ColumnBase> column = m_table->at(index);
    column->sort(order);
    emit layoutChanged();
}

/*!
 * \brief 按指定列排序数据表.
 * \param column 列索引
 * \param order 顺序
 */
void DataModel::sortTableByColumn(int column, Qt::SortOrder order)
{
    if (m_table->sortByColumn(column, order))
        emit layoutChanged();
}

/*!
 * \brief 设置行数.
 * \param count 行数
 */
void DataModel::setRowCount(int count)
{
    m_modelStyle->modelRowCount = count;
    emit layoutChanged();
}

/*!
 * \brief 设置字体.
 * \param font 字体
 */
void DataModel::setFont(const QFont& font)
{
    m_modelStyle->font = font;
}

/*!
 * \brief 设置字体颜色.
 * \param clr 颜色
 */
void DataModel::setFontColor(const QColor& clr)
{
    m_modelStyle->fontColor = clr;
}

/*!
 * \brief 设置单元格的背景颜色.
 * \param clr 颜色
 */
void DataModel::setBackgroundColor(const QColor& clr)
{
    m_modelStyle->backgroundColor = clr;
}

/*!
 * \brief 设置数据表格的显示格式.
 * \param format 格式
 */
void DataModel::setDisplayFormat(char format)
{
    m_modelStyle->format = format;
}

/*!
 * \brief 设置指定列的数据显示格式.
 * \param column 列索引
 * \param format 显示格式
 */
void DataModel::setColumnFormat(int column, char format)
{
    m_modelStyle->setColumnFormat(column, format);
}

/*!
 * \brief 获取列的数据显示格式.
 * \param column 列
 * \return  显示格式
 */
char DataModel::columnFormat(int column)
{
    return m_modelStyle->columnFormat(column);
}

/*!
 * \brief 设置数据表的显示精度.
 * \param precision 精度
 */
void DataModel::setPrecision(int precision)
{
    m_modelStyle->precision = precision;
}

/*!
 * \brief 获取数据表的显示精度.
 * \return 精度
 */
int DataModel::precision() const
{
    return m_modelStyle->precision;
}

/*!
 * \brief 设置列的显示精度.
 * \param column 列索引
 * \param precision 精度
 */
void DataModel::setColumnPrecision(int column, int precision)
{
    m_modelStyle->setColumnPrecision(column, precision);
    emit layoutChanged();
}

/*!
 * \brief 获取列的显示精度.
 * \param column 列索引
 * \return 精度
 */
int DataModel::columnPrecision(int column)
{
    return m_modelStyle->columnPrecision(column);
}

void DataModel::applySetting()
{
    m_modelStyle->applySetting();
}

/*!
 * \brief 清空表格.
 */
void DataModel::clear()
{
    m_table->clear();
    emit layoutChanged();
}

/*!
 * \brief 设置表对象.
 * \param t 表对象指针
 */
void DataModel::setTable(Table *t)
{
    if (!t)
        return;

    TableModelBase::setTable(t);
    m_table = t;

    disconnect(m_table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), this ,SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)));
    disconnect(m_table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), this, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
    connect(m_table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), this ,SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)));
    connect(m_table, SIGNAL(columnDataChanged(int,const ComData::ColumnBase*)), this, SLOT(onColumnDataChanged(int,const ComData::ColumnBase*)));
}

} // namespace ComDataGui
