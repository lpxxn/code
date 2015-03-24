#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include "comguiutils_global.h"

#include <QObject>
#include <QColor>

class QPushButton;

namespace GuiUtils {

class COMGUIUTILS_EXPORT ColorButton : public QObject
{
    Q_OBJECT
public:
    ColorButton(QPushButton*, const QColor& color);
    virtual ~ColorButton();

    QColor color() const { return m_color; }
    void setColor(const QColor&);

public slots:
    void clicked();

private:
    QColor m_color;
    QPushButton* m_btn;
};

}

#endif // COLORBUTTON_H
