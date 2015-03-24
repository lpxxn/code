#ifndef PSV_3PIEITEM_H
#define PSV_3PIEITEM_H
#include <QGraphicsItem>
#include "psv_item.h"

/*!
  \class PSV_Pie3Item
  \brief ��άƴͼ
  \author miaozhengwei QQ��393758926
  \version 1.0.1
  \date 2014-03-10 �޸�
*/
class PSV_Pie3Item : public PSV_Item
{
public:
    PSV_Pie3Item(const QMap<PSV::ATTRIBUTE_ROLE, QVariant> &param, QGraphicsItem * parent = 0 );
    ~PSV_Pie3Item();

    virtual int type () const;
    void setChartData(const QMap<QString, double>& data);
    void hideLabel();
    void showLabel();

    void setThickness(int thickness);
    void setStartAngle(int startAngle);
    void setCounterClockwise(bool counter);
    void setViewAngle(int angle);
    void setRadiusRatio(float ratio);
    void setDisplacementRatio(float ratio);

protected:
    void initVariables(/*const QMap<PSV::PARAM_EKY, QVariant> &param*/);
    void updateItem();
    virtual void setItemNull();
    void adjustChartRect();

protected:
    bool m_isShowLabel;
    bool m_isCounterClockwise;
    int m_thickness;
    int m_viewAngle;
    qreal m_startAngle;
    qreal m_heigthRatio;
    float m_radiusRatio;
    float m_displacementRatio;
    double m_sum;
    QRectF m_chartRect;
    QFont m_labelFont;
    QMap<QString, double> m_data;
};
#endif // PSV_3PIEITEM_H
