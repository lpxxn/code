#ifndef COLUMNPROPERTYDATA
#define COLUMNPROPERTYDATA

#include "comdatagui_global.h"

#include <comdata/column.h>

namespace ComDataGui
{
class COMDATAGUI_EXPORT ColumnPropertyData
{
public:
    explicit ColumnPropertyData();
    ~ColumnPropertyData();
    ColumnPropertyData(const ColumnPropertyData &);
    ColumnPropertyData operator=(const ColumnPropertyData& );

    void setColumn(int column);
    int column() const;

    void setPrecision(int precision);
    int precision() const;

    void setSignificance(ComData::ColumnBase::Significance sig);
    ComData::ColumnBase::Significance significance() const;

    void setColumnType(ComData::ColumnBase::ColumnType ct);
    ComData::ColumnBase::ColumnType columnType() const;

    void setLongName(const QString& longName);
    QString longName() const;

    void setShortName(const QString& shortName);
    QString shortName() const;

    void setUnit(const QString& unit);
    QString unit() const;

    void setUseDecimal(bool use);
    bool useDecimal() const;

    bool isValid() const;

    friend inline bool operator==(const ColumnPropertyData& lhs, const ColumnPropertyData& rhs);

private:
    int m_column = -1;
    int m_precision;
    ComData::ColumnBase::Significance m_significance;
    ComData::ColumnBase::ColumnType m_columnType;
    QString m_longName;
    QString m_shortName;
    QString m_unit;
    bool m_useDecimal;
};

inline bool operator ==(const ColumnPropertyData& lhs, const ColumnPropertyData& rhs)
{
    return lhs.m_column == rhs.m_column &&
            lhs.m_precision == rhs.m_precision &&
            lhs.m_significance == rhs.m_significance &&
            lhs.m_columnType == rhs.m_columnType &&
            lhs.m_longName == rhs.m_longName &&
            lhs.m_shortName == rhs.m_shortName &&
            lhs.m_unit == rhs.m_unit &&
            lhs.m_useDecimal == rhs.m_useDecimal;
}

}
#endif // COLUMNPROPERTYDATA

