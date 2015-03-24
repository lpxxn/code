#ifndef TCTEXTRULER_H
#define TCTEXTRULER_H

#include "comguiutils_global.h"

#include <QWidget>

class QPainter;
class QMouseEvent;

class COMGUIUTILS_EXPORT TcTextRuler : public QWidget
{
    Q_OBJECT
public:
    explicit TcTextRuler(QWidget *parent = nullptr);
    ~TcTextRuler();

public Q_SLOTS:
    void setTextWidth(qreal textWidth);
    void setColumn(int column);
    void setSpacing(int emptySpacing);
    void setStartPosition(int xPos);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    void drawBg(QPainter* painter);
    void drawMarks(QPainter* painter);
    void drawIndicator(QPainter* painter);

    class PrivateData;
    PrivateData* mData;
};

#endif // TCTEXTRULER_H
