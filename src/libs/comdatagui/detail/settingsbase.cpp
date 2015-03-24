#include "settingsbase.h"

SettingsBase::SettingsBase(QObject *parent) :
    GuiUtils::IOptionsPage(parent)
{
    connect(this, SIGNAL(activate()), SLOT(onActivate()));
}
