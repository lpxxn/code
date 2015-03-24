#ifndef OPTIONSPAGELIST_H
#define OPTIONSPAGECOLLECTION_H

#include "comguiutils_global.h"

#include <comutils/id.h>

#include <QString>
#include <QList>
#include <QHash>

namespace GuiUtils {

class IOptionsPage;

class COMGUIUTILS_EXPORT OptionsPageCollection
{
public:
    static OptionsPageCollection* instance();
    ~OptionsPageCollection();

    bool addOptionsPage(CommonUtils::Id app, IOptionsPage* page);
    IOptionsPage* optionsPage(CommonUtils::Id app, CommonUtils::Id id) const;

    QList<IOptionsPage*> optionsPages(CommonUtils::Id app) const { return m_pages[app]; }

private:
    QHash<CommonUtils::Id, QList<IOptionsPage*>> m_pages;
    OptionsPageCollection();
};

}

#endif // OPTIONSPAGECOLLECTION_H
