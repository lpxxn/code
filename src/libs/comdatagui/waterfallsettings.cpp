#include "waterfallsettings.h"
#include "ui_waterfallsettings.h"
#include "constants.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>

WaterfallSettings::WaterfallSettings(QObject *parent) :
    SettingsBase(parent)
{
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Waterfall"));
}

QWidget *WaterfallSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::WaterfallSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    return m_widget;
}

void WaterfallSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

void WaterfallSettings::finish()
{
    if (!m_ui) // page was never shown
        return;

    delete m_ui;
    m_ui = nullptr;

    release();
}

void WaterfallSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget(curve());
}
