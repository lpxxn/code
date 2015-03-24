/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SCALE_WIDGET_H
#define QWT_SCALE_WIDGET_H

#include "qwt_global.h"
#include "qwt_text.h"
#include "qwt_scale_draw.h"
#include <qwidget.h>
#include <qfont.h>
#include <qcolor.h>
#include <qstring.h>

class QPainter;
class QwtTransform;
class QwtScaleDiv;
class QwtColorMap;

/*!
  \brief A Widget which contains a scale

  This Widget can be used to decorate composite widgets with
  a scale.
*/

class QWT_EXPORT QwtScaleWidget : public QWidget
{
    Q_OBJECT

public:
    //! Layout flags of the title
    enum LayoutFlag
    {
        /*!
          The title of vertical scales is painted from top to bottom. 
          Otherwise it is painted from bottom to top.
         */
        TitleInverted = 1
    };

    //! Layout flags of the title
    typedef QFlags<LayoutFlag> LayoutFlags;

    explicit QwtScaleWidget( QWidget *parent = NULL );
    explicit QwtScaleWidget( QwtScaleDraw::Alignment, QWidget *parent = NULL );
    virtual ~QwtScaleWidget();

Q_SIGNALS:
    //! Signal emitted, whenever the scale division changes
    void scaleDivChanged();

public:
    void setTitle( const QString &title );
    void setTitle( const QwtText &title );
    QwtText title() const;

    void setTitleVisible(bool visible);
    bool isTitleVisible() const;

    void setShown(bool visible);
    bool isShown() const;

    void setLabelColor(const QColor& clr);
    QColor labelColor() const;

    void setTickColor(const QColor& clr);
    QColor tickColor() const;

    void setTickWidth(int width);
    int tickWidth() const;

    void setLabelFont(const QFont& f);
    QFont labelFont() const;

    void setMajorLabelsVisible(bool visible);
    bool isMajorLabelsVisible() const;

    void setMinorLabelsVisible(bool visible);
    bool isMinorLabelsVisible() const;

    void setPlusSignVisible(bool visible);
    bool isPlusSignVisible() const;

    void setMinusSignVisible(bool visible);
    bool isMinusSignVisible() const;

    void setPrefix(const QString& prefix);
    QString prefix() const;

    void setSuffix(const QString& suffix);
    QString suffix() const;

    void setDecimalWidth(int width);
    int decimalWidth() const;

    void setAxisNotation(QwtAbstractScaleDraw::Notation notation);
    QwtAbstractScaleDraw::Notation axisNotation() const;

    void setLayoutFlag( LayoutFlag, bool on );
    bool testLayoutFlag( LayoutFlag ) const;

    void setBorderDist( int start, int end );
    int startBorderDist() const;
    int endBorderDist() const;

    void getBorderDistHint( int &start, int &end ) const;

    void getMinBorderDist( int &start, int &end ) const;
    void setMinBorderDist( int start, int end );

    void setMargin( int );
    int margin() const;

    void setSpacing( int td );
    int spacing() const;

    void setScaleDiv( const QwtScaleDiv &sd );
    void setTransformation( QwtTransform * );

    void setScaleDraw( QwtScaleDraw * );
    const QwtScaleDraw *scaleDraw() const;
    QwtScaleDraw *scaleDraw();

    void setLabelAlignment( Qt::Alignment );
    void setLabelRotation( double rotation );

    void setColorBarEnabled( bool );
    bool isColorBarEnabled() const;

    void setColorBarWidth( int );
    int colorBarWidth() const;

    void setColorMap( const QwtInterval &, QwtColorMap * );

    QwtInterval colorBarInterval() const;
    const QwtColorMap *colorMap() const;

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    int titleHeightForWidth( int width ) const;
    int dimForLength( int length, const QFont &scaleFont ) const;

    void drawColorBar( QPainter *painter, const QRectF & ) const;
    void drawTitle( QPainter *painter, QwtScaleDraw::Alignment,
        const QRectF &rect ) const;

    void setAlignment( QwtScaleDraw::Alignment );
    QwtScaleDraw::Alignment alignment() const;

    QRectF colorBarRect( const QRectF& ) const;

protected:
    virtual void paintEvent( QPaintEvent * );
    virtual void resizeEvent( QResizeEvent * );

    void draw( QPainter *p ) const;

    void scaleChange();
    void layoutScale( bool update = true );

private:
    void initScale( QwtScaleDraw::Alignment );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtScaleWidget::LayoutFlags )

#endif