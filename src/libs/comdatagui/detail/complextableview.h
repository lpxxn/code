#ifndef COMPLEXTABLEVIEW_H
#define COMPLEXTABLEVIEW_H

#include "tableviewbase.h"

#include <QTableView>
#include <QDebug>

namespace ComData
{
    class ColumnBase;
}

class QPushButton;

namespace ComDataGui
{

class FilterDialog;
class AdvancedFunctionDialog;
class ThumbnailWidget;

typedef enum { FT_Less, FT_LessEqual, FT_Larger, FT_LargerEqual,
               FT_Equal, FT_NotEqual, FT_Top, FT_Bottom } FilterType;

class FilterData
{
public:
    FilterData() {}
    enum Operator { None, And, Or };

    FilterData operator=(const FilterData& other)
    {
        type1 = other.type1;
        type2 = other.type2;
        threshhold1 = other.threshhold1;
        threshhold2 = other.threshhold2;
        op = other.op;
        return *this;
    }

    void print()
    {
        qDebug() << "type1: " << type1;
        qDebug() << "type2: " << type2;
        qDebug() << "threshhold1: " << threshhold1;
        qDebug() << "threshhold2: " << threshhold2;
        qDebug() << "op: " << op;
    }

    QString assign(FilterType ft, const QString& arg) const
    {
        QString s;
        switch (ft) {
        case FT_Less:
            s = "<" + arg;
            break;

        case FT_LessEqual:
            s = "<=" + arg;
            break;

        case FT_Larger:
            s = ">" + arg;
            break;

        case FT_LargerEqual:
            s = ">=" + arg;
            break;

        case FT_Equal:
            s = "=" + arg;
            break;

        case FT_NotEqual:
            s = "!=" + arg;
            break;

        case FT_Top:
            s = "Top" + arg;
            break;

        case FT_Bottom:
            s = "Bottom" + arg;
            break;
        default:
            s = arg;
        }

        return s;
    }

    QString append(FilterType ft, const QString& ori, const QString& arg) const
    {
        QString s(ori);
        switch (ft) {
        case FT_Less:
            s += "<" + arg;
            break;

        case FT_LessEqual:
            s += "<=" + arg;
            break;

        case FT_Larger:
            s += ">" + arg;
            break;

        case FT_LargerEqual:
            s += ">=" + arg;
            break;

        case FT_Equal:
            s += "=" + arg;
            break;

        case FT_NotEqual:
            s += "!=" + arg;
            break;

        case FT_Top:
            s += "Top" + arg;
            break;

        case FT_Bottom:
            s += "Bottom" + arg;
            break;

        default:
            s += arg;
        }
        return s;
    }

    QString toString() const
    {
        QString filter;

        switch (type1) {
        case FT_Less:
            filter = assign(FT_Less, QString("%1").arg(threshhold1));
            break;

        case FT_LessEqual:
            filter = assign(FT_LessEqual, QString("%1").arg(threshhold1));
            break;

        case FT_Larger:
            filter = assign(FT_Larger, QString("%1").arg(threshhold1));
            break;

        case FT_LargerEqual:
            filter = assign(FT_LargerEqual, QString("%1").arg(threshhold1));
            break;

        case FT_Equal:
            filter = assign(FT_Equal, QString("%1").arg(threshhold1));
            break;

        case FT_NotEqual:
            filter = assign(FT_NotEqual, QString("%1").arg(threshhold1));
            break;

        case FT_Top:
            filter = assign(FT_Top, QString("%1").arg(threshhold1));
            break;

        case FT_Bottom:
            filter = assign(FT_Bottom, QString("%1").arg(threshhold1));
            break;
        }

        if (type2 >= 0) {
            if (op == And)
                filter += " & ";
            else
                filter += " | ";

            switch (type2) {
            case FT_Less:
                filter = append(FT_Less, filter, QString("%1").arg(threshhold2));
                break;

            case FT_LessEqual:
                filter = append(FT_LessEqual, filter, QString("%1").arg(threshhold2));
                break;

            case FT_Larger:
                filter = append(FT_Larger, filter, QString("%1").arg(threshhold2));
                break;

            case FT_LargerEqual:
                filter = append(FT_LargerEqual, filter, QString("%1").arg(threshhold2));
                break;

            case FT_Equal:
                filter = append(FT_Equal, filter, QString("%1").arg(threshhold2));
                break;

            case FT_NotEqual:
                filter = append(FT_NotEqual, filter, QString("%1").arg(threshhold2));
                break;

            case FT_Top:
                filter = append(FT_Top, filter, QString("%1").arg(threshhold2));
                break;

            case FT_Bottom:
                filter = append(FT_Bottom, filter, QString("%1").arg(threshhold2));
                break;
            }
        }
        return filter;
    }

    int type1 = -1;
    int type2 = -1;
    double threshhold1 = 0;
    double threshhold2 = 0;
    Operator op;
};

class ComplexTableView : public TableViewBase
{
    Q_OBJECT
public:
    explicit ComplexTableView(QWidget *parent = 0);

    void setLongName(int column, const QString& longName);
    void setUnit(int column, const QString& unit);
    void swapColumn(int from, int to);
    void moveColumn(int from, int to);

public slots:
    void onColumnDataChanged(int column, const ComData::ColumnBase* columnData);
    void removeThumbnails();

signals:
    void dataChanged(const QModelIndex& index, const QString& section, const QVariant& data);
    void evaluate(const QModelIndex& index, const QString& expression);
    void filterOut(const QModelIndex& index, const FilterData& fd);
    void resetColumn(const QModelIndex& index);
    void horizontalHeaderClicked(int index);
    void setFilter(const QModelIndex& index, const QString& filter);
    void updateMinSize();

protected:
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    void keyPressEvent(QKeyEvent* e);

protected slots:
    virtual void columnResized(int column, int oldWidth, int newWidth);

private slots:
    void onIndexDoubleClicked(const QModelIndex& index);
    void onSetFilter(const FilterData& fd);
    void showAdvancedDialog();
    void onSetExpression(const QString& exp);

protected:
    virtual void commitData(QWidget* editor);
    virtual void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);

private:
    void init();
    void setCellData(int column, const QString& verticalHeaderLabel, const QVariant& data);
    int rowOfLabel(const QString& verticalLabel);
    void updatePreviewSize();

private:
    typedef QMap<QModelIndex, int> IndexTypeMap;
    IndexTypeMap m_indexTypeMap;

    typedef QMap<int, ThumbnailWidget*> ColumnThumbnailMap;
    ColumnThumbnailMap m_columnThumbnailMap;

    FilterDialog* m_filterDlg = nullptr;
    AdvancedFunctionDialog* m_advancedFuncDlg = nullptr;
    QModelIndex m_pressedIndex;
};

} // namespace ComDataGui

Q_DECLARE_METATYPE(ComDataGui::FilterData)

#endif // COMPLEXTABLEVIEW_H
