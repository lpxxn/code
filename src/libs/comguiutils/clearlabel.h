#ifndef CLEARLABEL_H
#define CLEARLABEL_H

#include "comguiutils_global.h"

#include <QLabel>
#include <QMouseEvent>

/*!
 * \class ClearLabel
 * \brief A QLabel class which could emit clicked signal
 */
class COMGUIUTILS_EXPORT ClearLabel: public QLabel
{
    Q_OBJECT
public:
    explicit ClearLabel(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev);

Q_SIGNALS:
    void clicked();
};

#endif // CLEARLABEL_H
