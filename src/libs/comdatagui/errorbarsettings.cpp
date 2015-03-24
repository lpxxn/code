#include "errorbarsettings.h"
#include "ui_errorbarsettings.h"
#include "constants.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>

ErrorBarSettings::ErrorBarSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_ERRORBAR);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Error Bar"));
}

/*!
 * \reimp
 */
QWidget *ErrorBarSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::ErrorBarSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);

    ComDataGui::Utils::populateSizeComboBox(m_ui->errorBarCapWidthCombo);
    ComDataGui::Utils::populateSizeComboBox(m_ui->errorBarLineWidthCombo);

    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void ErrorBarSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void ErrorBarSettings::finish()
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
void ErrorBarSettings::updateUi()
{
    m_ui->errorBarPlusCheck->setChecked(sGraphProperty->isErrorBarPlusVisible());
    m_ui->errorBarMinusCheck->setChecked(sGraphProperty->isErrorBarMinusVisible());
    m_ui->errorBarColorButton->setColor(sGraphProperty->errorBarLineColor());
    setComboBoxIndexByValue(m_ui->errorBarCapWidthCombo, sGraphProperty->errorBarHandleWidth());
    setComboBoxIndexByValue(m_ui->errorBarLineWidthCombo, sGraphProperty->errorBarLineWidth());
    m_ui->errorBarLineOpacitySlider->setValue(sGraphProperty->errorBarLineOpacity() * 100);
}

/*!
 * \reimp
 */
void ErrorBarSettings::initConns()
{
    connect(m_ui->errorBarPlusCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->errorBarMinusCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->errorBarCapWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->errorBarLineWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->errorBarLineOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->errorBarColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
}

/*!
 * \reimp
 */
void ErrorBarSettings::onComboBoxIndexChanged(int index)
{
    Q_UNUSED(index)
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->errorBarCapWidthCombo)
        sGraphProperty->setErrorBarHandleWidth(which->currentText().toInt());
    else if (which == m_ui->errorBarLineWidthCombo)
        sGraphProperty->setErrorBarLineWidth(which->currentText().toInt());

    applyToCurve();
    applyToBarChart();
}

/*!
 * \reimp
 */
void ErrorBarSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());

    if (which == m_ui->errorBarPlusCheck)
        sGraphProperty->setErrorBarPlusVisible(state == Qt::Checked);
    else if (which == m_ui->errorBarMinusCheck)
        sGraphProperty->setErrorBarMinusVisible(state == Qt::Checked);

    applyToBarChart();
    applyToCurve();
}

/*!
 * \reimp
 */
void ErrorBarSettings::onSliderValueChanged(int value)
{
    qreal opacity = (qreal)value / 100;
    sGraphProperty->setErrorBarLineOpacity(opacity);

    applyToCurve();
    applyToBarChart();
}

/*!
 * \reimp
 */
void ErrorBarSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showCurve();
}

void ErrorBarSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());
    const QColor& clr = which->color();
    sGraphProperty->setErrorBarLineColor(clr);

    applyToCurve();
    applyToBarChart();
}
