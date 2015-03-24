#ifndef PSV_PUBLIC_H
#define PSV_PUBLIC_H
#include <Qt>
#include <QVariant>
#include <QDateTime>
#include <QColor>

#define PSV_ZEOR 1.0E-16
#define PSV_PI 3.1415926

#define PSV_NOUSED(var) (void)var

const QDateTime PSV_BEGIN_DATETIME = QDateTime(QDate(1990,1,1),QTime(0,0,0));

class QColor;
namespace PSV
{
enum DATA_STRUCT{
    realNumber = 3001, 
    positiveNumber, 
    negativeNumber
};

enum ITEM_TYPE{
    psvItem = Qt::UserRole + 1//������
    , axisItem//����
    , axisUpItem
    , axisDownItem
    , axisLeftItem
    , axisRightItem
    , axisListItem
    , crcularIndicatorItem
    , cuboidItem
    , histogramItem
    , tableItem
    , sectorItem
    , chartItem//����
    , chartTimeItem
    , chartDateTimeItem
    , chartRealItem
    , chartHistogramItem
    , chartAreaItem
    , painterPathItem
    , chartBarItem
    , verticalIndicatorItem
    , treeItem
    , levelItem
    , buttonItem
};

enum ATTRIBUTE_ROLE{
   lineWidth = 1001
    , defaultColor
    , margin
    , lineColor//
    , boundingRect//
    , value//
    , maxValue//
    , minValue//
    , font//
    , valueFont
    , staFont//
    , data//
    , dataColor
    , dataFont
    , markFont//����̶�font
    , markLength//����̶ȳ���
    , markColor//����̶���ɫ
    , markWidth//����̶��ߵĿ��
    , arrowsLength//�������ͷ�ĳ���
    , upPhaseBrush//����
    , sidePhaseBrush//����
    , frontPhaseBrush//����
    , obliqueAngle//б��
    , cosLength//���ҳ���
    , titleText
    , titleFont
    , titleColor
    , vHeaderColor
    , hHeaderColor
    , pieItem
    , pie3Item
    , height
    , startAngle
    , angle
    , factor//darker
    , color
    , toolTip
    , levelInfos
    , indicatorWidthRatio
    , hasArrow
    , margin_up
    , margin_down
    , margin_left
    , margin_right
    , isSameColor
    , sameColor
    , dimension//ά��
    , isCuboid//�Ƿ�Ϊ������
    , isCenter
    , dx
    , dy
    , dw
    , dh
};

enum DATA_TYPE
{
    dataDouble
    ,dataTime
};
//===================================================================
/*!< \brief ������ʾ���ö�ٳ���
*/
enum CURVETYPE{
    E_CURVE_LINE = 1001 /*!< �������� */
    , E_CURVE_PARELLEL /*!< ƽֱ���� */
    , E_CURVE_SMOOTH /*!< �⻬���� */
    , E_CURVE_UNKOWN /*!< δ֪���� */
};

/*!< \brief X��������������ö�ٳ���
*/
enum DATATYPE{
    E_DATATYPE_TIME = 2001 /*!< QTime */
    , E_DATATYPE_DATE /*!< QDate */
    , E_DATATYPE_DOUBLE /*!< double */
    , E_DATATYPE_STRING /*!< QString */
    , E_DATATYPE_DATETIME /*!< QDateTime */
    , E_DATATYPE_LIST /*!< QList */
    , E_DATATYPE_UNKOWN /*!< δ֪���ͣ����ڳ�ʼ�� */
};

enum AXISTTYPE{
    E_AXIS_LEFT = 3001,
    E_AXIS_RIGHT,
    E_AXIS_DOWN,
    E_AXIS_UP
};
//===================================================================
}

class PSV_Public
{
public:
    PSV_Public();
    ~PSV_Public();
    static void printMes(const QVariant &mes,const QString &frefix = "");
    static int getNumTicks(double& max, double& min, int minimumNumTicks = 4); //��ȡ�ʵ��Ŀ̶���
    static QColor getColor(int index = -1);
    static QColor getHistogramColor(int index,const QList<QColor> &colorList,bool isSameColor = false,const QColor &sameColor = QColor(Qt::black));
    static void adjustRange(double &max, double &min);
    static bool getLabels(QVariant &maxValue, QVariant &minValue, QPair<double,double> &range, QList<QPair<QVariant, QString> > &labelList);
    static double getListValue(const QList<double> &valueList,const bool isStacked,bool &isNonnegative);
//    static QString getStaText(const QStringList &textList,const QFont &font,QList<QColor> colorList = QList<QColor>());
    static QString getStaText(const QStringList &textList,QList<QColor> colorList = QList<QColor>());
    static double getMaxLenght(const QRectF& rect);
    static double getMinLenght(const QRectF& rect);

protected:
    static bool getDateLabels(QDate &maxDate, QDate &minDate, QList<QPair<QVariant, QString> > &labelList);
    static bool getTimeLabels(QTime &maxTime, QTime &minTime, QList<QPair<QVariant, QString> > &labelList);
    static bool getDateTimeLabels(QDateTime &maxTime, QDateTime &minTime, QList<QPair<QVariant, QString> > &labelList);
};

#endif // PSV_PUBLIC_H
