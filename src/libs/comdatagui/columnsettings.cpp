#include "columnsettings.h"
#include "ui_columnsettings.h"
#include "constants.h"
#include "utils.h"

#include <comguiutils/guiconstants.h>
#include <comguiutils/colorcombobox.h>
#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>

ColumnSettings::ColumnSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_BARCHART);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Bar Chart"));
}

/*!
 * \reimp
 */
QWidget *ColumnSettings::createPage(QWidget *parent)
{
    init();

    m_ui = new Ui::ColumnSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    ComDataGui::Utils::loadBrushStyles(m_ui->fillPatternCombo);
    ComDataGui::Utils::populateSizeComboBox(m_ui->borderWidthCombo, 0);
    ComDataGui::Utils::populateSizeComboBox(m_ui->fillLineWidthCombo, 0, 20);

    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void ColumnSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void ColumnSettings::finish()
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
void ColumnSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showBarChart();
}

/*!
 * \reimp
 */
void ColumnSettings::onSliderValueChanged(int value)
{
    QSlider* which = qobject_cast<QSlider*>(sender());

    if (which == m_ui->fillOpacitySlider)
        sGraphProperty->setBarChartFillOpacity((float) value / 100);

    if (which == m_ui->labelOapcitySlider)
        sGraphProperty->setBarLabelOpacity((float) value / 100);

    applyToBarChart();
}

/*!
 * \reimp
 */
void ColumnSettings::onComboBoxIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->fillLineWidthCombo)
        sGraphProperty->setBarChartLineWidth(m_ui->fillLineWidthCombo->currentText().toInt());
    else if (which == m_ui->fillPatternCombo)
        setBarChartFillPattern(index);
    else if (which == m_ui->fillBarChartStyleCombo)
        sGraphProperty->setBarChartFillStyle(static_cast<QwtColumnSymbol::FrameStyle>(index));
    else if (which == m_ui->labelPositionCombo)
        setBarChartLabelPosition(index);
    else if (which == m_ui->borderWidthCombo) {
        int width = which->currentText().toInt();

        if (which == 0) {
            sGraphProperty->setBarChartBorderPen(Qt::NoPen);
        } else {
            QPen pen = sGraphProperty->barChartBorderPen();
            pen.setWidth(width);
            sGraphProperty->setBarChartBorderPen(pen);
        }

    }

    applyToBarChart();
}

void ColumnSettings::onFontSelected(const QFont& f)
{
    sGraphProperty->setBarLabelFont(f);
    applyToBarChart();
}

void ColumnSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->labelColorButton)
        sGraphProperty->setBarLabelColor(which->color());
    else if (which == m_ui->borderColorButton) {
        QPen pen = sGraphProperty->barChartBorderPen();
        pen.setColor(which->color());
        sGraphProperty->setBarChartBorderPen(pen);
    }

    applyToBarChart();
}

/*!
 * \reimp
 */
void ColumnSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->labelOffsetXSpinBox)
        sGraphProperty->setBarLabelOffsetX(value);
    else if (which == m_ui->labelOffsetYSpinBox)
        sGraphProperty->setBarLabelOffsetY(value);

    applyToBarChart();
}

void ColumnSettings::onGroupBoxClicked(bool check)
{
    sGraphProperty->setBarLabelVisible(check);
    applyToBarChart();
}

/*!
 * \reimp
 */
void ColumnSettings::initConns()
{
    connect(m_ui->fillOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->fillLineWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->fillPatternCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->fillBarChartStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->labelColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->labelFontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));
    connect(m_ui->labelOapcitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->labelOffsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->labelOffsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->labelGroupBox, SIGNAL(clicked(bool)), this, SLOT(onGroupBoxClicked(bool)));
    connect(m_ui->labelPositionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->borderWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->borderColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
}

/*!
 * \reimp
 */
void ColumnSettings::updateUi()
{
    m_ui->fillOpacitySlider->setValue(sGraphProperty->barChartFillOpacity() * 100);
    m_ui->fillLineWidthCombo->setCurrentIndex(sGraphProperty->barChartLineWidth() - 1);
    m_ui->fillPatternCombo->setCurrentIndex((int)sGraphProperty->barChartFillPattern());
    m_ui->fillBarChartStyleCombo->setCurrentIndex((int) sGraphProperty->barChartFillStyle());
    m_ui->labelColorButton->setColor(sGraphProperty->barLabelColor());
    m_ui->labelFontButton->setFont(sGraphProperty->barLabelFont());
    m_ui->labelOapcitySlider->setValue(sGraphProperty->barLabelOpacity() * 100);
    m_ui->labelOffsetXSpinBox->setValue(sGraphProperty->barLabelOffsetX());
    m_ui->labelOffsetYSpinBox->setValue(sGraphProperty->barLabelOffsetY());
    m_ui->labelGroupBox->setChecked(sGraphProperty->isBarLabelVisible());
    m_ui->labelPositionCombo->setCurrentIndex((int) sGraphProperty->barLabelPosition());
    m_ui->borderColorButton->setColor(sGraphProperty->barChartBorderPen().color());
    m_ui->borderWidthCombo->setCurrentIndex(sGraphProperty->barChartBorderPen().width() - 1);
}

void ColumnSettings::setBarChartFillPattern(int pattern)
{
    switch (pattern) {
    case 0:
        sGraphProperty->setBarChartFillPattern(Qt::NoBrush);
        break;

    case 1:
        sGraphProperty->setBarChartFillPattern(Qt::SolidPattern);
        break;

    case 2:
        sGraphProperty->setBarChartFillPattern(Qt::Dense1Pattern);
        break;

    case 3:
        sGraphProperty->setBarChartFillPattern(Qt::Dense2Pattern);
        break;

    case 4:
        sGraphProperty->setBarChartFillPattern(Qt::Dense3Pattern);
        break;

    case 5:
        sGraphProperty->setBarChartFillPattern(Qt::Dense4Pattern);
        break;

    case 6:
        sGraphProperty->setBarChartFillPattern(Qt::Dense5Pattern);
        break;

    case 7:
        sGraphProperty->setBarChartFillPattern(Qt::Dense6Pattern);
        break;

    case 8:
        sGraphProperty->setBarChartFillPattern(Qt::Dense7Pattern);
        break;

    case 9:
        sGraphProperty->setBarChartFillPattern(Qt::HorPattern);
        break;

    case 10:
        sGraphProperty->setBarChartFillPattern(Qt::VerPattern);
        break;

    case 11:
        sGraphProperty->setBarChartFillPattern(Qt::CrossPattern);
        break;

    case 12:
        sGraphProperty->setBarChartFillPattern(Qt::BDiagPattern);
        break;

    case 13:
        sGraphProperty->setBarChartFillPattern(Qt::FDiagPattern);
        break;

    case 14:
        sGraphProperty->setBarChartFillPattern(Qt::DiagCrossPattern);
        break;
    }
}

void ColumnSettings::setBarChartLabelPosition(int position)
{
    sGraphProperty->setBarLabelPosition(static_cast<QwtPlot::LabelPosition>(position));
}
