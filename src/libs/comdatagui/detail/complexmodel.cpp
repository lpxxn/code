#include "complexmodel.h"
#include "complextableview.h"

#include <comutils/settingsmanager.h>
#include <comdata/table.h>
#include <comdatagui/constants.h>

#include <QDebug>
#include <QAbstractItemView>
#include <QPushButton>
#include <QTableView>
#include <QTableWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QSettings>
#include <QCoreApplication>

namespace ComDataGui
{

ComplexModel::ComplexModel(QObject *parent) :
    TableModelBase(parent), m_verticalLabelVisible(true)
{
    initVerticalLabels();
}

ComplexModel::ComplexModel(const ComplexModel &complexModel)
{
    m_horizontalHeaderLabels = complexModel.horizontalHeaderLabels();
    m_backgroundColor = complexModel.backgroundColor();
    m_verticalLabelVisible = complexModel.isVerticalLabelVisible();
}

/*!
 * \reimp
 */
int ComplexModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_verticalHeaderLabels.count();
}

/*!
 * \reimp
 */
int ComplexModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_horizontalHeaderLabels.count();
}

/*!
 * \reimp
 */
QVariant ComplexModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        int col = index.column();
        int row = index.row();
        QSharedPointer<ComData::ColumnBase> cb = m_table->at(col);
        if (!cb.isNull()) {
            if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME)) == row)
                return QVariant(cb->longName());
            else if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT)) == row)
                return QVariant(cb->unit());
            else if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT)) == row)
                return QVariant(cb->comment());
            else if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)) == row)
                return QVariant(cb->expression());
            else if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)) == row)
                return cb->expression();
            else if (verticalLabelIndex(QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER)) == row) {
                QVariant v = cb->extra(0);
                FilterData fd = v.value<FilterData>();
                return fd.toString();
            }
        }
    }

    if (role == Qt::BackgroundRole)
        return QBrush(m_backgroundColor);
    else if (role == Qt::SizeHintRole) {
        return QSize(200, 200);
    }

    return QVariant::Invalid;
}

/*!
 * \reimp
 */
Qt::ItemFlags ComplexModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

/*!
 * \reimp
 */
QVariant ComplexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((section < 0) ||
        ((orientation == Qt::Horizontal) && (section >= columnCount())) ||
        ((orientation == Qt::Vertical) && (section >= rowCount())))
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical && !m_verticalHeaderLabels.isEmpty()) {
            if (!m_verticalLabelVisible)
                return QVariant();

            if (section < m_verticalHeaderLabels.size())
                return m_verticalHeaderLabels.at(section);
            else
                return QAbstractItemModel::headerData(section, orientation, role);
        } else if (orientation == Qt::Horizontal && !m_horizontalHeaderLabels.isEmpty()){
            if (role == Qt::SizeHintRole) {
                QSize size(3, 30);
                return QVariant(size);
            }

            if (section < m_horizontalHeaderLabels.size())
                return m_horizontalHeaderLabels.at(section);
            else
                return QAbstractItemModel::headerData(section, orientation, role);
        }
    } else if (role == Qt::SizeHintRole && orientation == Qt::Horizontal) {
        QSize size(13, 30);
        size.setWidth(50);
        return QVariant(size);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

/*!
 * \reimp
 */
bool ComplexModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return QAbstractItemModel::setHeaderData(section, orientation, value, role);
}

/*!
 * \brief 获取竖直标签是否可见.
 * \return
 */
bool ComplexModel::isVerticalLabelVisible() const
{
    return m_verticalLabelVisible;
}

/*!
 * \brief 设置竖直标签是否可见.
 * \param visible 是否可见
 */
void ComplexModel::setVerticalLabelVisible(bool visible)
{
    m_verticalLabelVisible = visible;
    emit layoutChanged();
}

void ComplexModel::applySettings()
{
    //initVerticalLabel();
    CommonUtils::SettingsManager* sm = CommonUtils::SettingsManager::instance();
    QSettings *setting = sm->generalSettings();

    setRowVisible(setting, QStringLiteral("Table/LongNameVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME));
    setRowVisible(setting, QStringLiteral("Table/UnitVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT));
    setRowVisible(setting, QStringLiteral("Table/CommentVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT));
    setRowVisible(setting, QStringLiteral("Table/UserVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION));
    setRowVisible(setting, QStringLiteral("Table/FilterVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER, false));
    setRowVisible(setting, QStringLiteral("Table/SparklinesVisibility"), QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
    m_backgroundColor = setting->value(QStringLiteral("Table/TableHeaderBackground"),
                                       QColor(Qt::yellow)).value<QColor>();
    emit layoutChanged();
}

void ComplexModel::setRowVisible(const QSettings* setting, const QString& settingStr, const QString& rowName, bool isvisibility)
{    
    if (!setting->value(settingStr, isvisibility).toBool())
        emit rowVisible(rowName, false);
}

/*!
 * \brief 初始化竖直标签.
 */
void ComplexModel::initVerticalLabels()
{
    setVerticalHeaderLabels(QStringList()
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWLONGNAME)
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUNIT)
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWCOMMENT)
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWUSERFUNCTION)
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWFILTER)
                            << QCoreApplication::translate("SimViz", ComDataGui::Constants::TABLEROWTHUNMBNAIL));
}

/*!
 * \brief 根据标签名获取标签的行号.
 * \param label 标签名
 * \return 行号
 */
int ComplexModel::verticalLabelIndex(const QString &label) const
{
    int index = -1;
    for (int i = 0; i < rowCount(); ++i) {
        QString vhl = headerData(i, Qt::Vertical, Qt::DisplayRole).toString();
        if (vhl.compare(label) == 0) {
            index = i;
            break;
        }
    }
    return index;
}

} // namespace ComDataGui
