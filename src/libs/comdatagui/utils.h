#ifndef UTILS_H
#define UTILS_H

#include "comdatagui_global.h"

#include <QObject>
#include <QEvent>
#include <QComboBox>

#include <qwt/qwt_symbol.h>

namespace ComDataGui
{

namespace Utils
{

class COMDATAGUI_EXPORT EnterNotification : public QObject
{
    Q_OBJECT
public:
    explicit EnterNotification(QObject* object): m_object(object) {}

signals:
    void entered();

protected:
    bool eventFilter(QObject *obj, QEvent *e)
    {
        if (obj == m_object && e->type() == QEvent::Enter)
            emit entered();
        return QObject::eventFilter(obj, e);
    }

    QObject* m_object;
};

enum GradientType
{
    None,
    TopBottom,
    BottomTop,
    LeftRight,
    RightLeft,
    DiagonalTLBR,
    DiagonalTRBL,
    VerticalCenterOut,
    VerticalCenterIn,
    HorizontalCenterOut,
    HorizontalCenterIn,
    DiagonalUpCenterOut,
    DiagonalDownCenterOut,
    FromCornerTopLeft,
    FromCornerTopRight,
    FromCornerBottomRight,
    FromCornerBottomLeft,
//    FromCenter,
    GradientCount
};

QIcon COMDATAGUI_EXPORT createIconByColor(const QColor& clr);
QIcon COMDATAGUI_EXPORT createGradientIcon(GradientType gt, const QColor& clr);
QIcon COMDATAGUI_EXPORT createGradientIcon(GradientType gt, const QColor& firstClr,
                                           const QColor& secondClr);
QIcon COMDATAGUI_EXPORT createBrushStyleIcon(Qt::BrushStyle bs);
QIcon COMDATAGUI_EXPORT createPenStyleIcon(Qt::PenStyle ps);
QIcon COMDATAGUI_EXPORT createSymbolIcon(QwtSymbol::Style style);
QLinearGradient COMDATAGUI_EXPORT createGradientByType(GradientType gt, const QRect& rect,
                                                       const QColor& clr);
QLinearGradient COMDATAGUI_EXPORT createGradientByTypeEx(GradientType gt, const QRect& rect,
                                       const QColor& firstClr, const QColor& secondClr);
QString COMDATAGUI_EXPORT gradientName(GradientType gt);
void COMDATAGUI_EXPORT loadBrushStyles(QComboBox* cb);
void COMDATAGUI_EXPORT loadPenStyles(QComboBox* cb);
void COMDATAGUI_EXPORT populateSizeComboBox(QComboBox* cb, int min = 1, int max = 5);
void COMDATAGUI_EXPORT populateGradientComboBox(QComboBox* cb, const QColor& clr);
void COMDATAGUI_EXPORT populateGradientComboBoxEx(QComboBox *cb, const QColor &firstClr,
                                                  const QColor& secondClr);

}
}
#endif // UTILS_H
