#include "graphwindowsettings.h"
#include "ui_graphwindowsettings.h"
#include "constants.h"
#include "utils.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>

#include <QCheckBox>

GraphWindowSettings::GraphWindowSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_GRAPHWINDOW);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Graph Window"));
}

GraphWindowSettings::~GraphWindowSettings()
{
}

/*!
 * \reimp
 */
QWidget *GraphWindowSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::GraphWindowSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);

    ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo, sGraphProperty->backgroundStartColor());
    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void GraphWindowSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void GraphWindowSettings::finish()
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
void GraphWindowSettings::initConns()
{
    connect(m_ui->gradientFillCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
    connect(m_ui->directionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
    connect(m_ui->firstColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->secondColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
//    connect(m_ui->connectAxisBreakCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->lightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
}

/*!
 * \reimp
 */
void GraphWindowSettings::updateUi()
{
    m_ui->firstColorButton->setColor(sGraphProperty->backgroundStartColor());
    m_ui->secondColorButton->setColor(sGraphProperty->backgroundStopColor());
    m_ui->gradientFillCombo->setCurrentIndex(sGraphProperty->gradientFillMode());
//    m_ui->connectAxisBreakCheck->setChecked(m_graphProperty->isConnectAxisBreak());
    m_ui->lightnessSlider->setValue(sGraphProperty->backgroundLightness());
    m_ui->directionCombo->setCurrentIndex(sGraphProperty->backgroundGradientFill());
    updateUiByGradientFillMode(sGraphProperty->gradientFillMode());
}

/*!
 * \reimp
 */
void GraphWindowSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*) sDemoWidget);
    showCurve();
}

/*!
 * \reimp
 */
void GraphWindowSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());

//    if (which == m_ui->connectAxisBreakCheck)
//        m_graphProperty->setConnectAxisBreak(state == Qt::Checked);
//    else if (which == m_ui->connectMissingDataCheck)
        sGraphProperty->setConnectMissingData(state == Qt::Checked);

    makeDirty();
    VizGraphBaseSettings::apply();
}

void GraphWindowSettings::onComboBoxCurrentIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->gradientFillCombo) {
        sGraphProperty->setGradientFillMode(index);
        updateUiByGradientFillMode(index);
    } else if (which == m_ui->directionCombo) {
        sGraphProperty->setBackgroundGradientFill((ComDataGui::Utils::GradientType) index);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

void GraphWindowSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->firstColorButton) {
        sGraphProperty->setBackgroundStartColor(which->color());
        int index = m_ui->directionCombo->currentIndex();
        if (index == 2)
            sGraphProperty->setBackgroundStopColor(QColor());
        m_ui->directionCombo->blockSignals(true);
        if (m_ui->gradientFillCombo->currentIndex() != 2)
            ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo, which->color());
        else
            ComDataGui::Utils::populateGradientComboBoxEx(m_ui->directionCombo, m_ui->firstColorButton->color(), m_ui->secondColorButton->color());
        m_ui->directionCombo->blockSignals(false);
        m_ui->directionCombo->setCurrentIndex(index);
    } else if (which == m_ui->secondColorButton) {
        sGraphProperty->setBackgroundStopColor(which->color());
        int index = m_ui->directionCombo->currentIndex();
        m_ui->directionCombo->blockSignals(true);
        ComDataGui::Utils::populateGradientComboBoxEx(m_ui->directionCombo, m_ui->firstColorButton->color(), m_ui->secondColorButton->color());
        m_ui->directionCombo->blockSignals(false);
        m_ui->directionCombo->setCurrentIndex(index);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void GraphWindowSettings::onSliderValueChanged(int value)
{
    sGraphProperty->setBackgroundLightness(value);
    QColor clr = m_ui->firstColorButton->color();
    QColor secondClr = clr.light(value);
    sGraphProperty->setBackgroundStopColor(secondClr);
    makeDirty();
    VizGraphBaseSettings::apply();
}

void GraphWindowSettings::updateUiByGradientFillMode(int mode)
{
    switch (mode) {
    case 0:
        // invalidate the second background color
        sGraphProperty->setBackgroundStopColor(QColor());
        m_ui->lightnessLabel->hide();
        m_ui->lightnessSlider->hide();
        m_ui->secondColorButton->hide();
        m_ui->secondColorLabel->hide();
        m_ui->directionLabel->hide();
        m_ui->directionCombo->hide();
        break;

    case 1:
    {
        // invalidate the second background color
        QColor clr = m_ui->firstColorButton->color();
        QColor secondClr = clr.light(m_ui->lightnessSlider->value());
        sGraphProperty->setBackgroundStopColor(secondClr);
        m_ui->lightnessLabel->show();
        m_ui->lightnessSlider->show();
        m_ui->secondColorLabel->hide();
        m_ui->secondColorButton->hide();
        m_ui->directionLabel->show();
        m_ui->directionCombo->show();
        sGraphProperty->setBackgroundLightness(m_ui->lightnessSlider->value());
        int index = m_ui->directionCombo->currentIndex();
        ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo, m_ui->firstColorButton->color());
        m_ui->directionCombo->setCurrentIndex(index);
    }
        break;

    case 2:
    {
        m_ui->lightnessLabel->hide();
        m_ui->lightnessSlider->hide();
        m_ui->secondColorLabel->show();
        m_ui->secondColorButton->show();
        m_ui->directionLabel->show();
        m_ui->directionCombo->show();
        sGraphProperty->setBackgroundStopColor(m_ui->secondColorButton->color());
        int index = m_ui->directionCombo->currentIndex();
        ComDataGui::Utils::populateGradientComboBoxEx(m_ui->directionCombo, m_ui->firstColorButton->color(), m_ui->secondColorButton->color());
        m_ui->directionCombo->setCurrentIndex(index);
    }
        break;
    }
}
