#ifndef PSV_SECTORITEM_H
#define PSV_SECTORITEM_H
#include <QGraphicsItem>
#include "psv_item.h"

/*!
  \class PSV_SectorItem
  \brief ��άƴͼ��һƬ
  \author miaozhengwei QQ��393758926
  \version 1.0.1
  \date 2014-03-10 �޸�
*/
class PSV_SectorItem : public PSV_Item
{
public:
    PSV_SectorItem(const QMap<PSV::ATTRIBUTE_ROLE, QVariant> &param, QGraphicsItem * parent = 0 );
    ~PSV_SectorItem();

    virtual int type () const;
    void setDisplacementRatio(float ratio);
    void updateItem();

protected:
    void initVariables();
    void setItemNull();

    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) ;
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) ;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) ;

private:
    qreal m_height;
    qreal m_startAngle;
    qreal m_angle;
    qreal m_displacementRatio;

    QColor m_color;

    int m_factor;

    double m_dx;
    double m_dy;
};

#endif // PSV_SECTORITEM_H
