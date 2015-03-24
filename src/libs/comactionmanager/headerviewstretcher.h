#ifndef HEADERVIEWSTRETCHER_H
#define HEADERVIEWSTRETCHER_H

#include "comactionmanager_global.h"

#include <QObject>

class QHeaderView;

namespace GuiUtils {

class COMACTIONMANAGER_EXPORT HeaderViewStretcher : public QObject
{
    const int m_columnToStretch;
public:
    explicit HeaderViewStretcher(QHeaderView *headerView, int columnToStretch);

    virtual bool eventFilter(QObject *obj, QEvent *ev);
};

}

#endif // HEADERVIEWSTRETCHER_H
