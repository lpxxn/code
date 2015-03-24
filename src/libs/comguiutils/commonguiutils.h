#ifndef COMMONGUIUTILS_H
#define COMMONGUIUTILS_H

#include "comguiutils_global.h"

#include <QObject>

class QString;
class QWidget;

namespace GuiUtils {

class COMGUIUTILS_EXPORT WaitCursor {
public:
    WaitCursor();
    ~WaitCursor();
};

class COMGUIUTILS_EXPORT CommonGuiUtils {
public:
    static void information(QWidget *parent, const QString &title,
            const QString &text, const QString &detailedText = QString());
    static void warning(QWidget *parent, const QString &title,
            const QString &text, const QString &detailedText = QString());
    static void critical(QWidget *parent, const QString &title,
            const QString &text, const QString &detailedText = QString());
    static bool question(QWidget *parent, const QString &title,
            const QString &text, const QString &detailedText = QString(),
            const QString &yesText=QObject::tr("&Yes"),
            const QString &noText=QObject::tr("&No"));
    static QString createHtmlTooltip(const QStringList &tooltip);

private:
    CommonGuiUtils();
};

}


#endif // COMMONGUIUTILS_H
