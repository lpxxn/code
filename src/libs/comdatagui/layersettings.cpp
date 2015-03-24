#include "layersettings.h"
#include "ui_layersettings.h"
#include "constants.h"
#include "utils.h"

#include <comguiutils/colorpushbutton.h>
#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>

#include <QFontDialog>

LayerSettings::LayerSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_GRAPH);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Graph Layer"));
}

LayerSettings::~LayerSettings()
{
}

/*!
 * \reimp
 */
QWidget *LayerSettings::createPage(QWidget *parent)
{
    init();

    m_ui = new Ui::LayerSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo,
                                                sGraphProperty->canvasStartColor());
    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void LayerSettings::apply()
{
    if (!m_ui)
        return;

    sGraphProperty->setHorizontalTitleVisible(m_ui->xLabelCheckBox->isChecked());
    sGraphProperty->setVerticalTitleVisible(m_ui->yLabelCheckBox->isChecked());
    sGraphProperty->setTitleOpacity((qreal)m_ui->titleOpacitySlider->value() / 100);

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void LayerSettings::finish()
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
void LayerSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showCurve();
}

/*!
 * \reimp
 */
void LayerSettings::onComboBoxIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->directionCombo)
        sGraphProperty->setCanvasGradientFill((ComDataGui::Utils::GradientType) index);
    else if (which == m_ui->gradientFillCombo) {
        sGraphProperty->setCanvasGradientFillMode(index);
        updateUiByGradientFillMode(index);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void LayerSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());

    if (which == m_ui->xLabelCheckBox)
        sGraphProperty->setHorizontalTitleVisible(state == Qt::Checked);
    else if (which == m_ui->yLabelCheckBox)
        sGraphProperty->setVerticalTitleVisible(state == Qt::Checked);
    else if (which == m_ui->showTitleCheck) {
        sGraphProperty->setTitleVisible(state == Qt::Checked);
        m_ui->titleTextEdit->setEnabled(state == Qt::Checked);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

void LayerSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->startColorButton) {
        const QColor& clr = m_ui->startColorButton->color();
        sGraphProperty->setCanvasStartColor(clr);
        int index = m_ui->directionCombo->currentIndex();
        m_ui->directionCombo->blockSignals(true);
        ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo, clr);
        m_ui->directionCombo->blockSignals(false);
        m_ui->directionCombo->setCurrentIndex(index);
    } else if (which == m_ui->stopColorButton) {
        const QColor& startClr = m_ui->startColorButton->color();
        const QColor& stopClr = m_ui->stopColorButton->color();
        sGraphProperty->setCanvasStartColor(startClr);
        sGraphProperty->setCanvasStopColor(stopClr);
        int index = m_ui->directionCombo->currentIndex();
        m_ui->directionCombo->blockSignals(true);
        ComDataGui::Utils::populateGradientComboBoxEx(m_ui->directionCombo, startClr, stopClr);
        m_ui->directionCombo->blockSignals(false);
        m_ui->directionCombo->setCurrentIndex(index);
    } else if (which == m_ui->titleColorButton) {
        const QColor& clr = m_ui->titleColorButton->color();
        sGraphProperty->setTitleColor(clr);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void LayerSettings::onSliderValueChanged(int value)
{
    QSlider* which = qobject_cast<QSlider*>(sender());

    if (which == m_ui->titleOpacitySlider)
        sGraphProperty->setTitleOpacity((qreal)value / 100);
    else if (which == m_ui->lightnessSlider) {
        sGraphProperty->setCanvasLightness(value);
        QColor clr = m_ui->startColorButton->color();
        QColor secondClr = clr.light(value);
        sGraphProperty->setCanvasStopColor(secondClr);
    }

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void LayerSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->borderRadiusSpinBox)
        sGraphProperty->setCanvasBorderRadius(value);

    makeDirty();
    VizGraphBaseSettings::apply();
}

void LayerSettings::onTextChanged(const QString &text)
{
    sGraphProperty->setTitleText(text);
    makeDirty();
    VizGraphBaseSettings::apply();
}

void LayerSettings::onFontSelected(const QFont &f)
{
    sGraphProperty->setTitleFont(f);
    makeDirty();
    VizGraphBaseSettings::apply();
}

void LayerSettings::updateUiByGradientFillMode(int mode)
{
    switch (mode) {
    case 0:
        // invalidate the second background color
        sGraphProperty->setCanvasStopColor(QColor());
        m_ui->lightnessLabel->hide();
        m_ui->lightnessSlider->hide();
        m_ui->stopColorButton->hide();
        m_ui->secondColorLabel->hide();
        m_ui->directionLabel->hide();
        m_ui->directionCombo->hide();
        break;

    case 1:
    {
        // invalidate the second background color
        QColor clr = m_ui->startColorButton->color();
        QColor secondClr = clr.light(m_ui->lightnessSlider->value());
        sGraphProperty->setCanvasStopColor(secondClr);
        m_ui->lightnessLabel->show();
        m_ui->lightnessSlider->show();
        m_ui->secondColorLabel->hide();
        m_ui->stopColorButton->hide();
        m_ui->directionLabel->show();
        m_ui->directionCombo->show();
        sGraphProperty->setCanvasLightness(m_ui->lightnessSlider->value());
        int index = m_ui->directionCombo->currentIndex();
        ComDataGui::Utils::populateGradientComboBox(m_ui->directionCombo, m_ui->startColorButton->color());
        m_ui->directionCombo->setCurrentIndex(index);
    }
        break;

    case 2:
    {
        m_ui->lightnessLabel->hide();
        m_ui->lightnessSlider->hide();
        m_ui->secondColorLabel->show();
        m_ui->stopColorButton->show();
        m_ui->directionLabel->show();
        m_ui->directionCombo->show();
        sGraphProperty->setCanvasStopColor(m_ui->stopColorButton->color());
        int index = m_ui->directionCombo->currentIndex();
        ComDataGui::Utils::populateGradientComboBoxEx(m_ui->directionCombo, m_ui->startColorButton->color(), m_ui->stopColorButton->color());
        m_ui->directionCombo->setCurrentIndex(index);
    }
        break;
    }
}

/*!
 * \reimp
 */
void LayerSettings::initConns()
{
    connect(m_ui->xLabelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->yLabelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(m_ui->startColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->stopColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->titleColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));

    connect(m_ui->titleOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->lightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->titleTextEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
    connect(m_ui->showTitleCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(m_ui->fontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));
    connect(m_ui->borderRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));

    connect(m_ui->gradientFillCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->directionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
}

/*!
 * \reimp
 */
void LayerSettings::updateUi()
{
    m_ui->startColorButton->setColor(sGraphProperty->canvasStartColor());
    m_ui->stopColorButton->setColor(sGraphProperty->canvasStopColor());
    m_ui->xLabelCheckBox->setChecked(sGraphProperty->isHorizontalTitleVisible());
    m_ui->yLabelCheckBox->setChecked(sGraphProperty->isVerticalTitleVisible());
    m_ui->showTitleCheck->setChecked(sGraphProperty->isTitleVisible());
    m_ui->fontButton->setFont(sGraphProperty->titleFont());
    m_ui->titleTextEdit->setText(sGraphProperty->titleText());
    m_ui->titleColorButton->setColor(sGraphProperty->titleColor());
    m_ui->titleOpacitySlider->setValue(sGraphProperty->titleOpacity() * 100);
    m_ui->borderRadiusSpinBox->setValue(sGraphProperty->canvasBorderRadius());
    m_ui->gradientFillCombo->setCurrentIndex(sGraphProperty->canvasGradientFillMode());
    m_ui->lightnessSlider->setValue(sGraphProperty->canvasLightness());
    updateUiByGradientFillMode(sGraphProperty->canvasGradientFillMode());
    m_ui->directionCombo->setCurrentIndex(sGraphProperty->canvasGradientFill());
}
