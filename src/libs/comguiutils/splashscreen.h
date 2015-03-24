#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "comguiutils_global.h"

#include <QSplashScreen>

namespace GuiUtils {

class COMGUIUTILS_EXPORT SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:    
    explicit SplashScreen(const QPixmap& pixmap = QPixmap());
    explicit SplashScreen(const QString& pixmapPath);

    void setFont(const QFont& font);
    void showMsg(const QString& msg, Qt::Alignment align = Qt::AlignLeft | Qt::AlignBottom, const QColor& color = Qt::red);

    // QSplashScreen interface
protected:
    void drawContents(QPainter *painter);

private:
    QPixmap m_pixmap;
    Qt::Alignment m_msgAlign;
    QFont m_font;
};

}

#endif // SPLASHSCREEN_H
