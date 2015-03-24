#include "utils.h"

#include <QPainter>
#include <QPixmap>

namespace ComDataGui
{

namespace Utils
{

/*!
 * \brief 创建指定颜色的图标.
 * \param clr 颜色
 * \return 图标
 */
QIcon createIconByColor(const QColor &clr)
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);
    painter.setPen(clr);
    painter.setBrush(clr);
    painter.drawRect(pixmap.rect());
    return QIcon(pixmap);
}

/*!
 * \brief 创建制定渐变类型和颜色的图标.
 * \param gt 渐变类型
 * \param clr 颜色
 * \return
 */
QIcon createGradientIcon(GradientType gt, const QColor& clr)
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);
    QLinearGradient gradient = createGradientByType(gt, pixmap.rect(), clr);
    painter.setBrush(gradient);
    painter.drawRect(pixmap.rect());
    return QIcon(pixmap);
}

/*!
 * \brief 创建制定渐变类型和起始、终止颜色的图标.
 * \param gt 渐变类型
 * \param firstClr 起始颜色
 * \param secondClr 终止颜色
 * \return
 */
QIcon createGradientIcon(GradientType gt, const QColor& firstClr, const QColor& secondClr)
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);
    QLinearGradient gradient = createGradientByTypeEx(gt, pixmap.rect(), firstClr, secondClr);
    painter.setBrush(gradient);
    painter.drawRect(pixmap.rect());
    return QIcon(pixmap);
}

/*!
 * \brief 创建制定画刷样式的图标.
 * \param bs 画刷样式
 * \return
 */
QIcon createBrushStyleIcon(Qt::BrushStyle bs)
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::black, bs));
    painter.drawRect(pixmap.rect());
    return QIcon(pixmap);
}

/*!
 * \brief 创建制定画笔风格的图标.
 * \param ps 画笔风格
 * \return
 */
QIcon createPenStyleIcon(Qt::PenStyle ps)
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);
    painter.save();
    painter.setBrush(Qt::white);
    painter.drawRect(pixmap.rect());
    painter.restore();

    if (ps != Qt::NoPen)
        painter.setPen(QPen(Qt::black, 1, ps));
    else
        painter.setPen(Qt::NoPen);
    painter.drawLine(QPoint(0, 16), QPoint(32, 16));
    return QIcon(pixmap);
}

/*!
 * \brief 创建制定符号类型的图标.
 * \param style 符号类型
 * \return
 */
QIcon createSymbolIcon(QwtSymbol::Style style)
{
    QPixmap icon = QPixmap(15, 15);
    QColor c = QColor (Qt::gray);
    icon.fill(c);
    const QPointF r = QPointF(7, 7);
    const QSize s = QSize(10, 10);
    QPainter p(&icon);
    p.setRenderHint(QPainter::Antialiasing);

    if (style != QwtSymbol::NoSymbol) {
        QwtSymbol symb;
        p.setBrush(QBrush(QColor(Qt::white)));
        symb.setStyle (style);
        symb.setSize(s);
        symb.drawSymbol(&p, r);
    }
    return icon;
}

template <class CreationPolicy>
struct GradientCreator
{
    static QLinearGradient create(const QRect& rect, const QColor& clr)
    {
        return CreationPolicy()(rect, clr);
    }

    static QLinearGradient create(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        return CreationPolicy()(rect, firstClr, secondClr);
    }
};

template <int type>
struct CreationPolicy
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        Q_UNUSED(rect)
        Q_UNUSED(clr)
        return QLinearGradient();
    }
};

template <int type>
struct CreationPolicyEx
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        Q_UNUSED(rect)
        Q_UNUSED(firstClr)
        Q_UNUSED(secondClr)
        return QLinearGradient();
    }
};

template <>
struct CreationPolicy<None>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr);
        gradient.setColorAt(1.0, clr);
        return gradient;
    }
};

template <>
struct CreationPolicyEx<None>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        Q_UNUSED(secondClr)
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<TopBottom>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<TopBottom>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<BottomTop>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<BottomTop>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<LeftRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<LeftRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<RightLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<RightLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<DiagonalTLBR>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<DiagonalTLBR>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<DiagonalTRBL>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<DiagonalTRBL>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<VerticalCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(0.5, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<VerticalCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(0.5, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<VerticalCenterIn>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(0.5, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<VerticalCenterIn>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(0.5, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<HorizontalCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(0.5, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<HorizontalCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(0.5, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<HorizontalCenterIn>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(0.5, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<HorizontalCenterIn>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.topRight());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(0.5, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<DiagonalUpCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(0.5, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<DiagonalUpCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(0.5, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<DiagonalDownCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(0.5, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<DiagonalDownCenterOut>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(0.5, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<FromCornerTopLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.center());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<FromCornerTopLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topLeft());
        gradient.setFinalStop(rect.center());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<FromCornerTopRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.center());
        gradient.setColorAt(0.0, clr.lighter());
        gradient.setColorAt(1.0, clr.dark());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<FromCornerTopRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.topRight());
        gradient.setFinalStop(rect.center());
        gradient.setColorAt(0.0, firstClr);
        gradient.setColorAt(1.0, secondClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<FromCornerBottomRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.center());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<FromCornerBottomRight>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.center());
        gradient.setFinalStop(rect.bottomRight());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

template <>
struct CreationPolicy<FromCornerBottomLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& clr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.center());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, clr.dark());
        gradient.setColorAt(1.0, clr.lighter());
        return gradient;
    }
};

template <>
struct CreationPolicyEx<FromCornerBottomLeft>
{
    QLinearGradient operator()(const QRect& rect, const QColor& firstClr, const QColor& secondClr)
    {
        QLinearGradient gradient;
        gradient.setStart(rect.center());
        gradient.setFinalStop(rect.bottomLeft());
        gradient.setColorAt(0.0, secondClr);
        gradient.setColorAt(1.0, firstClr);
        return gradient;
    }
};

QLinearGradient createGradientByType(GradientType gt, const QRect& rect, const QColor& clr)
{
    QLinearGradient gradient;
    switch (gt) {
    case None:
        gradient = GradientCreator<CreationPolicy<None> >::create(rect, clr);
        break;

    case TopBottom:
        gradient = GradientCreator<CreationPolicy<TopBottom> >::create(rect, clr);
        break;

    case BottomTop:
        gradient = GradientCreator<CreationPolicy<BottomTop> >::create(rect, clr);
        break;

    case LeftRight:
        gradient = GradientCreator<CreationPolicy<LeftRight> >::create(rect, clr);
        break;

    case RightLeft:
        gradient = GradientCreator<CreationPolicy<RightLeft> >::create(rect, clr);
        break;

    case DiagonalTLBR:
        gradient = GradientCreator<CreationPolicy<DiagonalTLBR> >::create(rect, clr);
        break;

    case DiagonalTRBL:
        gradient = GradientCreator<CreationPolicy<DiagonalTRBL> >::create(rect, clr);
        break;

    case VerticalCenterOut:
        gradient = GradientCreator<CreationPolicy<VerticalCenterOut> >::create(rect, clr);
        break;

    case VerticalCenterIn:
        gradient = GradientCreator<CreationPolicy<VerticalCenterIn> >::create(rect, clr);
        break;

    case HorizontalCenterOut:
        gradient = GradientCreator<CreationPolicy<HorizontalCenterOut> >::create(rect, clr);
        break;

    case HorizontalCenterIn:
        gradient = GradientCreator<CreationPolicy<HorizontalCenterIn> >::create(rect, clr);
        break;

    case DiagonalUpCenterOut:
        gradient = GradientCreator<CreationPolicy<DiagonalUpCenterOut> >::create(rect, clr);
        break;

    case DiagonalDownCenterOut:
        gradient = GradientCreator<CreationPolicy<DiagonalDownCenterOut> >::create(rect, clr);
        break;

    case FromCornerTopLeft:
        gradient = GradientCreator<CreationPolicy<FromCornerTopLeft> >::create(rect, clr);
        break;

    case FromCornerTopRight:
        gradient = GradientCreator<CreationPolicy<FromCornerTopRight> >::create(rect, clr);
        break;

    case FromCornerBottomRight:
        gradient = GradientCreator<CreationPolicy<FromCornerBottomRight> >::create(rect, clr);
        break;

    case FromCornerBottomLeft:
        gradient = GradientCreator<CreationPolicy<FromCornerBottomLeft> >::create(rect, clr);
        break;

//    case FromCenter:
//        gradient.setStart(rect.center());
//        gradient.setFinalStop(rect.center());
//        gradient.setColorAt(0.0, clr.dark());
//        gradient.setColorAt(1.0, clr.lighter());
        break;
    }

    return gradient;
}

QLinearGradient createGradientByTypeEx(GradientType gt, const QRect& rect,
                                       const QColor& firstClr, const QColor& secondClr)
{
    QLinearGradient gradient;
    switch (gt) {
    case None:
        gradient = GradientCreator<CreationPolicyEx<None> >::create(rect, firstClr, secondClr);
        break;

    case TopBottom:
        gradient = GradientCreator<CreationPolicyEx<TopBottom> >::create(rect, firstClr, secondClr);
        break;

    case BottomTop:
        gradient = GradientCreator<CreationPolicyEx<BottomTop> >::create(rect, firstClr, secondClr);
        break;

    case LeftRight:
        gradient = GradientCreator<CreationPolicyEx<LeftRight> >::create(rect, firstClr, secondClr);
        break;

    case RightLeft:
        gradient = GradientCreator<CreationPolicyEx<RightLeft> >::create(rect, firstClr, secondClr);
        break;

    case DiagonalTLBR:
        gradient = GradientCreator<CreationPolicyEx<DiagonalTLBR> >::create(rect, firstClr, secondClr);
        break;

    case DiagonalTRBL:
        gradient = GradientCreator<CreationPolicyEx<DiagonalTRBL> >::create(rect, firstClr, secondClr);
        break;

    case VerticalCenterOut:
        gradient = GradientCreator<CreationPolicyEx<VerticalCenterOut> >::create(rect, firstClr, secondClr);
        break;

    case VerticalCenterIn:
        gradient = GradientCreator<CreationPolicyEx<VerticalCenterIn> >::create(rect, firstClr, secondClr);
        break;

    case HorizontalCenterOut:
        gradient = GradientCreator<CreationPolicyEx<HorizontalCenterOut> >::create(rect, firstClr, secondClr);
        break;

    case HorizontalCenterIn:
        gradient = GradientCreator<CreationPolicyEx<HorizontalCenterIn> >::create(rect, firstClr, secondClr);
        break;

    case DiagonalUpCenterOut:
        gradient = GradientCreator<CreationPolicyEx<DiagonalUpCenterOut> >::create(rect, firstClr, secondClr);
        break;

    case DiagonalDownCenterOut:
        gradient = GradientCreator<CreationPolicyEx<DiagonalDownCenterOut> >::create(rect, firstClr, secondClr);
        break;

    case FromCornerTopLeft:
        gradient = GradientCreator<CreationPolicyEx<FromCornerTopLeft> >::create(rect, firstClr, secondClr);
        break;

    case FromCornerTopRight:
        gradient = GradientCreator<CreationPolicyEx<FromCornerTopRight> >::create(rect, firstClr, secondClr);
        break;

    case FromCornerBottomRight:
        gradient = GradientCreator<CreationPolicyEx<FromCornerBottomRight> >::create(rect, firstClr, secondClr);
        break;

    case FromCornerBottomLeft:
        gradient = GradientCreator<CreationPolicyEx<FromCornerBottomLeft> >::create(rect, firstClr, secondClr);
        break;

//    case FromCenter:
//        gradient.setStart(rect.center());
//        gradient.setFinalStop(rect.center());
//        gradient.setColorAt(0.0, clr.dark());
//        gradient.setColorAt(1.0, clr.lighter());
        break;
    }

    return gradient;
}

/*!
 * \brief 获取渐变类型对应的描述.
 * \param gt 渐变类型
 * \return
 */
QString gradientName(GradientType gt)
{
    QString name(QObject::tr("Invalid gradient"));

    switch (gt) {
    case None:
        name = QObject::tr("None");
        break;

    case TopBottom:
        name = QObject::tr("Top Bottom");
        break;

    case BottomTop:
        name = QObject::tr("Bottom Top");
        break;

    case LeftRight:
        name = QObject::tr("Left Right");
        break;

    case RightLeft:
        name = QObject::tr("Right Left");
        break;

    case DiagonalTLBR:
        name = QObject::tr("Diagonal TL BR");
        break;

    case DiagonalTRBL:
        name = QObject::tr("Diagonal TR BL");
        break;

    case VerticalCenterOut:
        name = QObject::tr("Vertical Center Out");
        break;

    case VerticalCenterIn:
        name = QObject::tr("Vertical Center In");
        break;

    case HorizontalCenterOut:
        name = QObject::tr("Horizontal Center Out");
        break;

    case HorizontalCenterIn:
        name = QObject::tr("Horizontal Center In");
        break;

    case DiagonalUpCenterOut:
        name = QObject::tr("Diagonal Up Center Out");
        break;

    case DiagonalDownCenterOut:
        name = QObject::tr("Diagonal Down Center Out");
        break;

    case FromCornerTopLeft:
        name = QObject::tr("From Corner Top Left");
        break;

    case FromCornerTopRight:
        name = QObject::tr("From Corner Top Right");
        break;

    case FromCornerBottomRight:
        name = QObject::tr("From Corner Bottom Right");
        break;

    case FromCornerBottomLeft:
        name = QObject::tr("From Corner Bottom Left");
        break;

//    case FromCenter:
//        name = QObject::tr("From Center");
//        break;
    }

    return name;
}

/*!
 * \brief 加载画刷类型到复合框中.
 * \param combo 复合框指针
 */
void loadBrushStyles(QComboBox* combo)
{
    combo->clear();
    combo->addItem(createBrushStyleIcon(Qt::NoBrush), QObject::tr("NoBrush"), (int)Qt::NoBrush);
    combo->addItem(createBrushStyleIcon(Qt::SolidPattern), QObject::tr("SolidPattern"), (int)Qt::SolidPattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense1Pattern), QObject::tr("Dense1Pattern"), (int)Qt::Dense1Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense2Pattern), QObject::tr("Dense2Pattern"), (int)Qt::Dense2Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense3Pattern), QObject::tr("Dense3Pattern"), (int)Qt::Dense3Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense4Pattern), QObject::tr("Dense4Pattern"), (int)Qt::Dense4Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense5Pattern), QObject::tr("Dense5Pattern"), (int)Qt::Dense5Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense6Pattern), QObject::tr("Dense6Pattern"), (int)Qt::Dense6Pattern);
    combo->addItem(createBrushStyleIcon(Qt::Dense7Pattern), QObject::tr("Dense7Pattern"), (int)Qt::Dense7Pattern);
    combo->addItem(createBrushStyleIcon(Qt::HorPattern), QObject::tr("HorPattern"), (int)Qt::HorPattern);
    combo->addItem(createBrushStyleIcon(Qt::VerPattern), QObject::tr("VerPattern"), (int)Qt::VerPattern);
    combo->addItem(createBrushStyleIcon(Qt::CrossPattern), QObject::tr("CrossPattern"), (int)Qt::CrossPattern);
    combo->addItem(createBrushStyleIcon(Qt::BDiagPattern), QObject::tr("BDiagPattern"), (int)Qt::BDiagPattern);
    combo->addItem(createBrushStyleIcon(Qt::FDiagPattern), QObject::tr("FDiagPattern"), (int)Qt::FDiagPattern);
    combo->addItem(createBrushStyleIcon(Qt::DiagCrossPattern), QObject::tr("DiagCrossPattern"), (int)Qt::DiagCrossPattern);
}

/*!
 * \brief 加载线型到复合框中.
 * \param combo 复合框指针
 */
void loadPenStyles(QComboBox* combo)
{
    combo->clear();
    combo->addItem(createPenStyleIcon(Qt::SolidLine), QObject::tr("Solid Line"), (int)Qt::SolidLine);
    combo->addItem(createPenStyleIcon(Qt::DashLine), QObject::tr("Dash Line"), (int)Qt::DashLine);
    combo->addItem(createPenStyleIcon(Qt::DotLine), QObject::tr("Dot Line"), (int)Qt::DotLine);
    combo->addItem(createPenStyleIcon(Qt::DashDotLine), QObject::tr("Dash Dot Line"), (int)Qt::DashDotLine);
    combo->addItem(createPenStyleIcon(Qt::DashDotDotLine), QObject::tr("Dash Dot Dot Line"), (int)Qt::DashDotDotLine);
    combo->addItem(createPenStyleIcon(Qt::CustomDashLine), QObject::tr("Custom Dash Line"), (int)Qt::CustomDashLine);
}

/*!
 * \brief 加载尺寸大小到复合框中.
 * \param combo 复合框指针
 * \param min 最小值
 * \param max 最大值
 */
void populateSizeComboBox(QComboBox *combo, int min, int max)
{
    combo->clear();
    for (int i = min; i <= max; ++i)
        combo->addItem(QString("%1").arg(i));
}

/*!
 * \brief 加载指定颜色的渐变到复合框中.
 * \param cb 复合框指针
 * \param clr 颜色
 */
void populateGradientComboBox(QComboBox *cb, const QColor& clr)
{
    cb->clear();
    for (int i = 0; i < GradientCount; ++i)
        cb->addItem(createGradientIcon((GradientType)i, clr), gradientName((GradientType)i), i);
}

/*!
 * \brief 加载指定起始和终止颜色的渐变到复合框中.
 * \param cb 复合框指针
 * \param firstClr 起始颜色
 * \param secondClr 终止颜色
 */
void populateGradientComboBoxEx(QComboBox *cb, const QColor &firstClr, const QColor& secondClr)
{
    cb->clear();
    for (int i = 0; i < GradientCount; ++i)
        cb->addItem(createGradientIcon((GradientType)i, firstClr, secondClr), gradientName((GradientType)i), i);
}

} // namespace Utils
} // namespace ComDataGui
