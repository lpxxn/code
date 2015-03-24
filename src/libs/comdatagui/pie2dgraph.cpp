#include "pie2dgraph.h"
#include "detail/psv_pie3item.h"

#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_item.h>
#include <qwt/qwt_scale_map.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_legend.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#define PI 3.1415926

namespace ComDataGui
{

class PieBaseItem : public QGraphicsItem
{
public:
    PieBaseItem(QGraphicsItem* item);
    virtual void refreshItem();
    bool clearAll();

    virtual void updateItem() = 0;
    virtual void setItemNull() = 0;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void advance(int phase);
};

PieBaseItem::PieBaseItem(QGraphicsItem *item): QGraphicsItem(item)
{

}

void PieBaseItem::refreshItem()
{
    if(clearAll())
        updateItem();
}

bool PieBaseItem::clearAll()
{
    QGraphicsScene *scene = this->scene();
    QList<QGraphicsItem*> childrenItems = this->childItems();
    if (scene != nullptr) {
        for(int i = 0; i < childrenItems.count(); ++i)
            scene->removeItem(childrenItems.at(i));
        setItemNull();
        return true;
    }
    return false;
}

void PieBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void PieBaseItem::advance(int phase)
{
    if(phase != 0)
        refreshItem();
}

class SectorItem : public PieBaseItem
{
public:
    SectorItem(QGraphicsItem * parent = 0 );
    ~SectorItem();

    virtual int type () const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setRect(const QRectF& rect);
    void setHeight(double height);
    void setStartAngle(qreal startAngle);
    void setAngle(qreal angle);
    void setColor(const QColor& clr);

    void updateItem();

protected:
    void initVariables();
    void setItemNull();

    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget *widget = 0);

private:
    QRectF m_rect;
    qreal m_height;
    qreal m_startAngle;
    qreal m_angle;
    QColor m_color;
    int m_factor;
    double m_dx;
    double m_dy;
    QPainterPath m_painterPath;
};

SectorItem::SectorItem(QGraphicsItem *parent)
    : PieBaseItem(parent),
      m_height(30),
      m_startAngle(270),
      m_angle(40),
      m_color(QColor(qrand() % 255, qrand() % 255, qrand() % 255)),
      m_factor(350),
      m_dx(0.0),
      m_dy(0.0)
{
    initVariables();
    updateItem();
    setAcceptHoverEvents(true);
}

SectorItem::~SectorItem()
{
}

int SectorItem::type() const
{
    return 1023;
}

QRectF SectorItem::boundingRect() const
{
    return m_rect;
}

QPainterPath SectorItem::shape() const
{
    return m_painterPath;
}

void SectorItem::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void SectorItem::setHeight(double height)
{
    m_height = height;
}

void SectorItem::setStartAngle(qreal startAngle)
{
    m_startAngle = startAngle;
}

void SectorItem::setAngle(qreal angle)
{
    m_angle = angle;
}

void SectorItem::setColor(const QColor& clr)
{
    m_color = clr;
}

void SectorItem::initVariables()
{
}

void SectorItem::updateItem()
{
    QRectF rect_up(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
    QRectF rect_down(m_rect.x(), m_rect.y() + m_height, m_rect.width(), m_rect.height());

    while (m_startAngle > 360)
        m_startAngle -= 360;

    double dx_e_up = rect_up.width() * 0.5 * cos((m_startAngle + m_angle)* PI / 180);
    double dy_e_up = rect_up.height() * 0.5 * sin((m_startAngle + m_angle)* PI / 180);
    double dx_e_down = rect_down.width() * 0.5 * cos((m_startAngle + m_angle)* PI / 180);
    double dy_e_down = rect_down.height() * 0.5 * sin((m_startAngle + m_angle)* PI / 180);

    double dx_s_up = rect_up.width() * 0.5 * cos(m_startAngle * PI / 180);
    double dy_s_up = rect_up.height() * 0.5 * sin(m_startAngle * PI / 180);
    double dx_s_down = rect_down.width() * 0.5 * cos(m_startAngle * PI / 180);
    double dy_s_down = rect_down.height() * 0.5 * sin(m_startAngle * PI / 180);

    QPointF pointS_U(rect_up.center().x() + dx_s_up, rect_up.center().y() - dy_s_up);
    QPointF pointS_D(rect_down.center().x() + dx_s_down, rect_down.center().y() - dy_s_down);
    QPointF pointE_U(rect_up.center().x() + dx_e_up, rect_up.center().y() - dy_e_up);
    QPointF pointE_D(rect_down.center().x() + dx_e_down, rect_down.center().y() - dy_e_down);

    m_dx = 0.1 * rect_up.width() * 0.5 * cos((m_startAngle + 0.5 * m_angle)* PI / 180);
    m_dy = -0.1 * rect_up.height() * 0.5 * sin((m_startAngle + 0.5 * m_angle)* PI / 180);

    QPen pen(m_color.dark(m_factor));
    {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect_up, this);
        item->setStartAngle((int)(m_startAngle * 16));
        item->setSpanAngle((int)(m_angle * 16));
        QBrush brush(m_color);
        item->setBrush(brush);
        item->setPen(pen);
        item->setZValue(4);
        m_painterPath.moveTo(rect_up.center());
        m_painterPath.arcTo(rect_up, m_startAngle, m_angle);
    }

    //item2
    {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect_down, this);
        item->setStartAngle((int)(m_startAngle * 16));
        item->setSpanAngle((int)(m_angle * 16));
        QBrush brush(m_color.dark(m_factor));
        item->setBrush(brush);
        item->setPen(pen);
    }

    int zValue_s = 1;
    int zValue_e = 2;
    if ((m_startAngle >= 0 && m_startAngle <= 90) ||
        (m_startAngle >= 270 && m_startAngle <= 360)) {
        zValue_s = 2;
        zValue_e = 1;
    }

    //south side
    // the vertical shadow at the top left corner
    {
        QPolygonF polygon;
        polygon.append(QPointF(pointS_U));
        polygon.append(QPointF(pointS_D));
        polygon.append(rect_down.center());
        polygon.append(rect_up.center());
        polygon.append(QPointF(pointS_U));
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon, this);
        item->setBrush(QBrush(m_color.dark(m_factor)));
        item->setPen(pen);
        item->setZValue(zValue_s);
    }

    //east side
    {
        QPolygonF polygon;
        polygon.append(QPointF(pointE_U));
        polygon.append(QPointF(pointE_D));
        polygon.append(rect_down.center());
        polygon.append(rect_up.center());
        polygon.append(QPointF(pointE_U));

        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon, this);
        item->setBrush(QBrush(m_color.dark(m_factor)));
        item->setPen(pen);
        item->setZValue(zValue_e);
    }
}

void SectorItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    moveBy(m_dx, m_dy);
}

void SectorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    moveBy(-m_dx, -m_dy);
}

void SectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPath(m_painterPath);
}

void SectorItem::setItemNull()
{
}

class PieItem : public PieBaseItem
{
public:
    PieItem(Graph* graph, QGraphicsItem * parent = 0 );
    ~PieItem();

    void setChartData(const QMap<QString, double>& data);
    void hideLabel();
    void showLabel();

    virtual int type () const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setRect(const QRectF& rect);
    void setThickness(int thickness);
    void setStartAngle(int startAngle);

protected:
    void initVariables(/*const QMap<PSV::PARAM_EKY, QVariant> &param*/);
    void updateItem();
    virtual void setItemNull();
    void adjustChartRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    bool m_isShowLabel;
    qreal m_startAngle;
    qreal m_heigthRatio;
    int m_thickness;
    double m_sum;
    QRectF m_chartRect;
    QFont m_labelFont;
    QRectF m_rect;
    QPainterPath m_painterPath;

    QColor m_defaultColor;
    QMap<QString, double> m_data;
    Graph* m_graph;
};

PieItem::PieItem(Graph* graph, QGraphicsItem *parent): PieBaseItem(parent),
    m_graph(graph),
    m_isShowLabel(true),
    m_startAngle(90),
    m_heigthRatio(0.2),
    m_thickness(40),
    m_sum(0)
{
    initVariables();
    updateItem();
}

PieItem::~PieItem()
{
    //
}

void PieItem::setItemNull()
{
    //
}

void PieItem::initVariables()
{
    m_defaultColor = Qt::black;
}

int PieItem::type() const
{
    return 1024;
}

QRectF PieItem::boundingRect() const
{
    return m_rect;
}

QPainterPath PieItem::shape() const
{
    return m_painterPath;
}

void PieItem::adjustChartRect()
{
#if 0
    if(m_isShowLabel) {
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
        m_chartRect = QRectF(m_rect.x(),m_rect.y()
                             ,m_rect.width() - width - height,m_rect.height());
    } else {
#endif
        m_chartRect = m_rect;
#if 0
    }
#endif
}

void PieItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void PieItem::setRect(const QRectF &rect)
{
    const QPointF c = rect.center();
    qreal radius = rect.width() > rect.height() ? rect.height() / 2 :
                                                  rect.width() / 2;
    //    radius /= 2;
    radius -= 30;
    QRectF adjustRect = QRectF(QPointF(c.x() - radius, c.y() - radius),
                               QPointF(c.x() + radius, c.y() + radius));
    m_rect = adjustRect;
    m_painterPath.moveTo(m_rect.center());
    m_painterPath.addRect(m_rect);
}

void PieItem::setThickness(int thickness)
{
    m_thickness = thickness;
}

void PieItem::setStartAngle(int startAngle)
{
    m_startAngle = startAngle;
}

void PieItem::updateItem()
{
    int count = m_data.count();
    if (count <= 0 || m_sum <= 0)
        return;

    m_labelFont.setPointSizeF(m_rect.width() * m_rect.height() * 0.000042);
    adjustChartRect();

    QMapIterator<QString, double> iter(m_data);
    int index = 0;
    //    qreal hei = m_heigthRatio * qMin(m_chartRect.width(),m_chartRect.height());
    // here could reset the thickness of the pie
    qreal startAngle = m_startAngle;
    qreal hei = m_thickness;
    m_chartRect = m_rect;
    int zValue = 0;

#if 0
    double dh = m_chartRect.height() / count;
    double curY = m_chartRect.top();
#endif

    m_graph->resetColorIndex();
    while(iter.hasNext()) {
        iter.next();
        qreal angle = iter.value() / m_sum * 360;
        const QColor color = m_graph->randomColor();
        SectorItem *item = new SectorItem(this);
        item->setRect(m_chartRect);
        item->setHeight(hei);
        item->setStartAngle(startAngle);
        item->setAngle(angle);
        item->setColor(color);
        item->setToolTip(iter.key());

        if ((startAngle >= 0 && startAngle<= 90) ||
                (startAngle >= 270 && startAngle<= 360))
            zValue--;
        else
            zValue++;

        item->setZValue(zValue);
        item->updateItem();
        startAngle += angle;
        while(startAngle > 360)
            startAngle -= 360;

#if 0
        if(m_isShowLabel) {
            QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
            textItem->setDefaultTextColor(m_defaultColor);
            textItem->setPlainText(iter.key());
            textItem->setFont(m_labelFont);
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
#endif
        index++;
    }
}

void PieItem::setChartData(const QMap<QString, double> &data)
{
    m_data = data;
    QMapIterator<QString, double> iter(m_data);
    m_sum = 0;
    while(iter.hasNext()) {
        iter.next();
        double value = iter.value();
        if(value < 0) {
            m_data.clear();
            return;
        } else {
            m_sum += value;
        }
    }
    refreshItem();
}

void PieItem::hideLabel()
{
    if(m_isShowLabel) {
        m_isShowLabel = false;
        refreshItem();
    }
}

void PieItem::showLabel()
{
    if(!m_isShowLabel) {
        m_isShowLabel = true;
        refreshItem();
    }
}

#if 0
class PiePlot : public QwtPlotSeriesItem, public QwtSeriesStore<QPointF>
{
public:
    explicit PiePlot(Graph* g);

    int rtti() const;

    void setSamples(const QVector<double>& data);

    virtual void drawSeries( QPainter *painter,
                             const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                             const QRectF &canvasRect, int from, int to ) const;

private:
    void init();
    void calcSum();
    void hideAxises();

private:
    Graph* m_graph;
    QVector<double> m_data;
    double m_sum;
};

/*!
 * \brief PieMarker::PieMarker constructor of PieMarker class
 */
PiePlot::PiePlot(Graph* g): m_graph(g)
{
    init();
}

int PiePlot::rtti() const
{
    return QwtPlotItem::Rtti_PlotUserItem + 2;
}

void PiePlot::setSamples(const QVector<double> &data)
{
    m_data = data;
    calcSum();
    dataChanged();
}

void PiePlot::drawSeries( QPainter *painter,
                          const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                          const QRectF &canvasRect, int from, int to ) const
{
    Q_UNUSED(xMap)
    Q_UNUSED(yMap)
    Q_UNUSED(from)
    Q_UNUSED(to)

    painter->save();
    //    painter->setRenderHints(QPainter::Antialiasing);

    int radius = canvasRect.width() > canvasRect.height() ? canvasRect.height() / 2 :
                                                            canvasRect.width() / 2;

    const QPointF c = canvasRect.center();
    QRectF pieRect = QRectF(QPointF(c.x() - radius, c.y() - radius), QPointF(c.x() + radius,
                                                                             c.y() + radius));
    painter->setPen(QPen(Qt::black, 1));

    qreal startAngle = 0;
    for (int i = 0; i < m_data.size(); ++i) {
        painter->setBrush(m_graph->randomColor());
        qreal angle = ((qreal)m_data.at(i) / m_sum) * ( 360 * 16 );
        painter->drawPie(pieRect, startAngle, angle);
        startAngle += angle;
    }

    painter->restore();
}

void PiePlot::init()
{
    hideAxises();
}

void PiePlot::calcSum()
{
    m_sum = 0;
    for (int i = 0; i < m_data.size(); ++i)
        m_sum += m_data.at(i);
}

void PiePlot::hideAxises()
{
    for ( int axisId = 0; axisId < QwtPlot::axisCnt; axisId++ )
    {
        QwtScaleWidget *scaleWidget = m_graph->axisWidget( axisId );
        scaleWidget->setVisible(false);
    }
}

#endif // if 0

class Pie2DGraph::Pie2DGraphPrivate
{
public:
    Pie2DGraphPrivate(): pie(nullptr)
    {
        view = new QGraphicsView;
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setRenderHint(QPainter::Antialiasing);
        scene = new QGraphicsScene;
        scene->setBackgroundBrush(QBrush(Qt::gray));
        view->setScene(scene);
    }

    QGraphicsView* view;
    QGraphicsScene* scene;
    PSV_Pie3Item* pie;
};

Pie2DGraph::Pie2DGraph(QWidget *parent) :
    Graph(parent)
{
    m_data = new Pie2DGraphPrivate;
    m_data->view->setParent(this);
    setCanvas(m_data->view);
}

Graph* Pie2DGraph::clone()
{
    return cloneGraph<Pie2DGraph>(this, m_columnData);
}

Graph* Pie2DGraph::rawClone()
{
    return cloneRawGraph<Pie2DGraph>(this);
}

void Pie2DGraph::initialize()
{
#if 0
    PiePlot* plot = new PiePlot(this);
    plot->attach(this);

    QVector<double> data;
    for (int i = 0; i < m_columnData.size(); ++i) {
        if (m_columnData.at(i)->type() == ComData::ColumnBase::DateTime ||
                m_columnData.at(i)->type() == ComData::ColumnBase::String)
            continue;

        ComData::ColumnBase* base = m_columnData.at(i);

        for (int index = 0; index < base->size(); ++index)
            data.append(base->cell(index).toDouble());
    }

    plot->setSamples(data);
    replot();
#endif

    if (m_data->pie == nullptr) {
        setTitle(tr("Pie"));
        insertLegend(new QwtLegend());
        QMap<PSV::ATTRIBUTE_ROLE, QVariant> param;
        QFont valueFont;
        QFont titleFont;
        valueFont.setPointSize(16);
        param.insert(PSV::boundingRect, QRectF(0, 0, 1200, 600));
        param.insert(PSV::valueFont, valueFont);
        param.insert(PSV::titleText, "Test");
        param.insert(PSV::titleFont, titleFont);
        param.insert(PSV::titleColor, QColor(Qt::blue));
        param.insert(PSV::value, 83);
        param.insert(PSV::maxValue, 100);
        param.insert(PSV::minValue, 0);
        m_data->pie = new PSV_Pie3Item(param);
    }
    m_data->scene->addItem(m_data->pie);
//    m_data->pie->clearAll();
//    m_data->pie->setRect(canvas()->rect());
    m_data->pie->hideLabel();

    QMap<QString, double> data;
    for (int i = 0; i < m_columnData.size(); ++i) {
        if (m_columnData.at(i)->type() == ComData::ColumnBase::DateTime ||
                m_columnData.at(i)->type() == ComData::ColumnBase::String)
            continue;

        QSharedPointer<ComData::ColumnBase> base = m_columnData.at(i);

        for (int index = 0; index < base->size(); ++index)
            data.insert(QString::number(index + 1), base->cell(index).toDouble());
    }

    m_data->pie->setChartData(data);
    m_data->pie->refreshItem();
}

void Pie2DGraph::dataUpdated()
{

}

void Pie2DGraph::clear()
{

}

void Pie2DGraph::addColumn(const PlotItemData& pid)
{

}

void Pie2DGraph::customize()
{
    setGridVisible(false);
    setAxisVisible(false);

    m_data->pie->setThickness(pieThickness());
    m_data->pie->setStartAngle(pieStartAngle());
    m_data->pie->setCounterClockwise(isPieCounterClockwise());
    m_data->pie->setViewAngle(pieViewAngle());
    m_data->pie->setRadiusRatio(pieRadiusRatio());
    m_data->pie->setDisplacementRatio(pieDisplacementRatio());
    m_data->pie->refreshItem();

    if (m_property.isLegendVisible())
        m_data->pie->showLabel();
    else
        m_data->pie->hideLabel();
}

int Pie2DGraph::type() const
{
    return 1001;
}

void Pie2DGraph::replot()
{
    customize();
}

void Pie2DGraph::resizeEvent(QResizeEvent *e)
{
    Graph::resizeEvent(e);
    if (m_data->pie) {
        m_data->scene->setSceneRect(canvas()->rect());
        m_data->scene->advance();
    }
}

}
