#include <QBrush>
#include <QPen>
#include <QFontMetrics>
#include<math.h>
#include "psv_pie3item.h"
#include "psv_sectoritem.h"
#include <QGraphicsSceneDragDropEvent>

PSV_Pie3Item::PSV_Pie3Item(const QMap<PSV::ATTRIBUTE_ROLE, QVariant> &param, QGraphicsItem *parent)
    : PSV_Item(param,parent)
    , m_isShowLabel(true)
    , m_thickness(30)
    , m_startAngle(90)
    , m_heigthRatio(0.2)
    , m_sum(0)
{
    initVariables();
    updateItem();
    m_painterPath.moveTo(m_rect.center());
    m_painterPath.addRect(m_rect);
    m_isAdvancetable = true;
}

PSV_Pie3Item::~PSV_Pie3Item()
{
}

void PSV_Pie3Item::setItemNull()
{
}

void PSV_Pie3Item::initVariables(/*const QMap<PSV::PARAM_EKY, QVariant> &m_param*/)
{
    if (m_param.contains(PSV::boundingRect))
        m_rect = m_param.value(PSV::boundingRect).toRectF();
    if (m_param.contains(PSV::toolTip))
        setToolTip(m_param.value(PSV::toolTip).toString());
    if (m_param.contains(PSV::data)) {
        QMap<QString, QVariant> tempData = m_param.value(PSV::data).toMap();
        QMapIterator<QString, QVariant> iter(tempData);
        while (iter.hasNext()) {
            iter.next();
            double value = iter.value().toDouble();
            if (value < 0) {
                m_data.clear();
                return;
            } else {
                m_data.insert(iter.key(), value);
                m_sum += value;
            }
        }
    }
}

int PSV_Pie3Item::type() const
{
    return PSV::pie3Item;
}

void PSV_Pie3Item::adjustChartRect()
{
    if (m_radiusRatio < 0)
        m_radiusRatio = 1;

    if (m_isShowLabel) {
        QString maxLengStr;
        QMap<QString, double>::const_iterator iter = m_data.constBegin();
        while(iter != m_data.constEnd()) {
            if(maxLengStr.toLocal8Bit().length() < iter.key().toLocal8Bit().length())
                maxLengStr = iter.key();
            ++iter;
        }
        QFontMetrics fontMetrics(m_labelFont);
        int width = fontMetrics.boundingRect(maxLengStr).width();
        int height = fontMetrics.boundingRect(maxLengStr).height();

        const QPointF c(m_rect.center().x() - width - height, m_rect.center().y());
        int radius = m_rect.width() > m_rect.height() ? m_rect.height() / 2 : m_rect.width() / 2;
        radius *= m_radiusRatio;
        int hei = sin(m_viewAngle * PSV_PI / 180) * radius;
        const QRectF r = QRectF(QPointF(c.x() - radius, c.y() - hei),
                                QPointF(c.x() + radius, c.y() + hei));

        m_chartRect = r;
        PSV_Public::printMes(width,"width");
    } else {
        const QPointF c = m_rect.center();
        int radius = m_rect.width() > m_rect.height() ? m_rect.height() / 2 : m_rect.width() / 2;
        radius *= m_radiusRatio;
        int hei = sin(m_viewAngle * PSV_PI / 180) * radius;
        QRectF r = QRectF(QPointF(c.x() - radius, c.y() - hei),
                          QPointF(c.x() + radius, c.y() + hei));
        m_rect = r;
        m_chartRect = m_rect;
    }
}

void PSV_Pie3Item::updateItem()
{
    int count = m_data.count();
    if(count <= 0 || m_sum <= 0)
        return;

    m_labelFont.setPointSizeF(m_rect.width() * m_rect.height() * 0.000042);
    adjustChartRect();

    QMapIterator<QString, double> iter(m_data);
    int index = 0;

    qreal startAngle = 0;
    if (m_isCounterClockwise)
        startAngle = 360 - m_startAngle;
    else
        startAngle = m_startAngle;

    QMap<PSV::ATTRIBUTE_ROLE, QVariant> param;
    param.insert(PSV::boundingRect, m_chartRect);
    param.insert(PSV::height, m_thickness);
    param.insert(PSV::factor, 200);
    int zValue = 0;
    double dh = m_chartRect.height() / count;
    double curY = m_chartRect.top();
    while(iter.hasNext()) {
        iter.next();
        QColor color = PSV_Public::getColor(index);
        qreal angle = iter.value() / m_sum * 360;
        param.insert(PSV::startAngle, startAngle);
        param.insert(PSV::angle, angle);
        param.insert(PSV::color, color);
        param.insert(PSV::toolTip, iter.key());

        PSV_SectorItem *item = new PSV_SectorItem(param, this);
        item->setDisplacementRatio(m_displacementRatio);
        item->updateItem();

        if((startAngle >= 0 && startAngle<= 90) || (startAngle >= 270 && startAngle<= 360))
            zValue--;
        else
            zValue++;
        item->setZValue(zValue);
        startAngle += angle;

        while(startAngle > 360)
            startAngle -= 360;

        if(m_isShowLabel) {
            QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
            textItem->setDefaultTextColor(m_defaultColor);
            textItem->setPlainText(iter.key());
            textItem->setFont(m_labelFont);
            textItem->setZValue(1001);
            double height = textItem->boundingRect().height() * 0.8;
            QRectF rect = QRectF(m_chartRect.right() + height
                                ,curY + 0.5 * dh - 0.5 * height
                                ,height,height);
            textItem->setPos(rect.right(),rect.y() - height * 0.1);
            QGraphicsRectItem* rectItem = new QGraphicsRectItem(rect,this);
            QBrush brush(color);
            QPen pen(color);
            rectItem->setPen(pen);
            rectItem->setBrush(brush);
            curY += dh;
        }
        index++;
    }    
}

void PSV_Pie3Item::setChartData(const QMap<QString, double> &data)
{
    m_data = data;
    QMapIterator<QString, double> iter(m_data);
    m_sum = 0;
    while(iter.hasNext()) {
        iter.next();
        double value = iter.value();
        if(value < 0) {
            m_data.clear();
            PSV_Public::printMes(QString("圆饼数据不能为负数<%1,%2>")
                                 .arg(iter.key())
                                 .arg(iter.value())
                                 ,"warning");
            return;
        } else {
            m_sum += value;
        }
    }
    refreshItem();
}

void PSV_Pie3Item::hideLabel()
{
    if(m_isShowLabel) {
        m_isShowLabel = false;
        refreshItem();
    }
}

void PSV_Pie3Item::showLabel()
{
    if(!m_isShowLabel) {
        m_isShowLabel = true;
        refreshItem();
    }
}

void PSV_Pie3Item::setThickness(int thickness)
{
    if (m_thickness != thickness) {
        m_thickness = thickness;
        refreshItem();
    }
}

void PSV_Pie3Item::setStartAngle(int startAngle)
{
    if (m_startAngle != startAngle) {
        m_startAngle = startAngle;
        refreshItem();
    }
}

void PSV_Pie3Item::setCounterClockwise(bool counter)
{
    if (m_isCounterClockwise != counter) {
        m_isCounterClockwise = counter;
        refreshItem();
    }
}

void PSV_Pie3Item::setViewAngle(int angle)
{
    if (m_viewAngle != angle) {
        m_viewAngle = angle;
        refreshItem();
    }
}

void PSV_Pie3Item::setRadiusRatio(float ratio)
{
    if (m_radiusRatio != ratio) {
        m_radiusRatio = ratio;
        refreshItem();
    }
}

void PSV_Pie3Item::setDisplacementRatio(float ratio)
{
    if (m_displacementRatio != ratio) {
        m_displacementRatio = ratio;
        refreshItem();
    }
}


