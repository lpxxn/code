#include "columnpropertydata.h"

namespace ComDataGui
{

ColumnPropertyData::ColumnPropertyData()
{
    m_column = -1;
}

ColumnPropertyData::~ColumnPropertyData()
{
}

ColumnPropertyData::ColumnPropertyData(const ColumnPropertyData &other)
{
    m_column = other.m_column;
    m_precision = other.m_precision;
    m_significance = other.m_significance;
    m_columnType = other.m_columnType;
    m_longName = other.m_longName;
    m_shortName = other.m_shortName;
    m_unit = other.m_unit;
    m_useDecimal = other.m_useDecimal;

}

ColumnPropertyData ColumnPropertyData::operator =(const ColumnPropertyData& other)
{
    m_column = other.m_column;
    m_precision = other.m_precision;
    m_significance = other.m_significance;
    m_columnType = other.m_columnType;
    m_longName = other.m_longName;
    m_shortName = other.m_shortName;
    m_unit = other.m_unit;
    m_useDecimal = other.m_useDecimal;
    return *this;
}

void ColumnPropertyData::setColumn(int column)
{
    m_column = column;
}

int ColumnPropertyData::column() const
{
    return m_column;
}

void ColumnPropertyData::setPrecision(int precision)
{
    m_precision = precision;
}

int ColumnPropertyData::precision() const
{
    return m_precision;
}

void ColumnPropertyData::setSignificance(ComData::ColumnBase::Significance sig)
{
    m_significance = sig;
}

ComData::ColumnBase::Significance ColumnPropertyData::significance() const
{
    return m_significance;
}

void ColumnPropertyData::setColumnType(ComData::ColumnBase::ColumnType ct)
{
    m_columnType = ct;
}

ComData::ColumnBase::ColumnType ColumnPropertyData::columnType() const
{
    return m_columnType;
}

void ColumnPropertyData::setLongName(const QString& longName)
{
    m_longName = longName;
}

QString ColumnPropertyData::longName() const
{
    return m_longName;
}

void ColumnPropertyData::setShortName(const QString& shortName)
{
    m_shortName = shortName;
}

QString ColumnPropertyData::shortName() const
{
    return m_shortName;
}

void ColumnPropertyData::setUnit(const QString& unit)
{
    m_unit = unit;
}

QString ColumnPropertyData::unit() const
{
    return m_unit;
}

void ColumnPropertyData::setUseDecimal(bool use)
{
    m_useDecimal = use;
}

bool ColumnPropertyData::useDecimal() const
{
    return m_useDecimal;
}

bool ColumnPropertyData::isValid() const
{
    return m_column != -1;
}


}
