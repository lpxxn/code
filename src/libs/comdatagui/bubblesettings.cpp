#include "bubblesettings.h"
#include "ui_bubblesettings.h"
#include "constants.h"
#include "utils.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>

BubbleSettings::BubbleSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_BUBBLES);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setCategoryIcon(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH_ICON);
    setDisplayName(tr("Bubbles"));
}

/*!
 * \reimp
 */
QWidget *BubbleSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::BubbleSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    ComDataGui::Utils::populateSizeComboBox(m_ui->sizeCombo, 1, 100);
    ComDataGui::Utils::populateSizeComboBox(m_ui->edgeWidthCombo, 0, 10);

    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void BubbleSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void BubbleSettings::finish()
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
void BubbleSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showBubble();
}

/*!
 * \reimp
 */
void BubbleSettings::onComboBoxTextChanged(const QString& text)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->sizeCombo)
        sGraphProperty->setBubbleSize(text.toInt());
    else if (which == m_ui->edgeWidthCombo)
        sGraphProperty->setBubbleEdgeWidth(text.toInt());

    applyToBubble();
}

/*!
 * \reimp
 */
void BubbleSettings::onSliderValueChanged(int value)
{
    sGraphProperty->setBubbleOpacity((float) value / 100);
    applyToBubble();
}

/*!
 * \reimp
 */
void BubbleSettings::updateUi()
{
    setComboBoxIndexByValue(m_ui->sizeCombo, sGraphProperty->bubbleSize());
    setComboBoxIndexByValue(m_ui->edgeWidthCombo, sGraphProperty->bubbleEdgeWidth());
    m_ui->opacitySlider->setValue(sGraphProperty->bubbleOpacity() * 100);
}

/*!
 * \reimp
 */
void BubbleSettings::initConns()
{
    connect(m_ui->sizeCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->edgeWidthCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
}
