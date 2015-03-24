#include "stacklinesettings.h"
#include "ui_stacklinesettings.h"
#include "constants.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>

StackLineSettings::StackLineSettings(QObject *parent) :
    SettingsBase(parent)
{
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Stack Lines"));
}

/*!
 * \reimp
 */
QWidget *StackLineSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::StackLineSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    return m_widget;
}

/*!
 * \reimp
 */
void StackLineSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void StackLineSettings::finish()
{
    if (!m_ui) // page was never shown
        return;
    delete m_ui;
    m_ui = nullptr;

    release();
}

/*!
 * \reimp
 */
void StackLineSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget(curve());
}
