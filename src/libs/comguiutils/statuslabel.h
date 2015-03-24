#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include "comguiutils_global.h"

#include <QLabel>

class QMenu;

namespace GuiUtils {

class COMGUIUTILS_EXPORT StatusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StatusLabel(const QString& text);
    void setMenu(QMenu* menu);

signals:
    void clicked();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QMenu* m_menu;
};

} //namespace GuiUtils


#endif // STATUSLABEL_H
