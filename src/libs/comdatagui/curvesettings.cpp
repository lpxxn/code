#include "curvesettings.h"
#include "ui_curvesettings.h"
#include "constants.h"
#include "utils.h"

#include <qwt/qwt_symbol.h>

#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>
#include <comguiutils/colorcombobox.h>

CurveSettings::CurveSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_CURVES);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Curves"));
}

QWidget *CurveSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::CurveSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);

    ComDataGui::Utils::loadPenStyles(m_ui->droplinePenStyleCombo);
    ComDataGui::Utils::populateSizeComboBox(m_ui->droplineWidthCombo);
    ComDataGui::Utils::populateSizeComboBox(m_ui->lineWidthCombo);
    ComDataGui::Utils::populateSizeComboBox(m_ui->symbolWidthCombo);

    populateSizeCombo();
    populateLabelPosCombo();

    updateUi();
    initConns();

    return m_widget;
}

void CurveSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

void CurveSettings::finish()
{
    if (!m_ui) // page was never shown
        return;

    delete m_ui;
    m_ui = nullptr;

    release();
}

void CurveSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showCurve();
}

void CurveSettings::onComboBoxTextChanged(const QString& text)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->lineWidthCombo)
        sGraphProperty->setCurveLineWidth(text.toInt());
    else if (which == m_ui->symbolCombo)
        sGraphProperty->setSymbol((QwtSymbol::Style) which->currentData().toInt());
    else if (which == m_ui->symbolSizeCombo)
        sGraphProperty->setSymbolSize(text.toInt());
    else if (which == m_ui->symbolWidthCombo)
        sGraphProperty->setSymbolWidth(text.toInt());
    else if (which == m_ui->droplineWidthCombo) {
        QPen pen = sGraphProperty->droplinePen();
        pen.setWidth(which->currentText().toInt());
        sGraphProperty->setDroplinePen(pen);
    } else if (which == m_ui->droplinePenStyleCombo) {
        QPen pen = sGraphProperty->droplinePen();
        pen.setStyle(static_cast<Qt::PenStyle>(which->currentData().toInt()));
        sGraphProperty->setDroplinePen(pen);
    }

    applyToCurve();
}

void CurveSettings::onComboBoxIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->labelPositionCombo)
        sGraphProperty->setCurveLabelPosition((QwtPlotCurve::LabelPosition) index);

    applyToCurve();
}

void CurveSettings::onSliderValueChanged(int value)
{
    QSlider* which = qobject_cast<QSlider*>(sender());

    if (which == m_ui->lineOpacitySlider)
        sGraphProperty->setCurveLineOpacity((float) value / 100);
    else if (which == m_ui->symbolOpacitySlider)
        sGraphProperty->setSymbolOpacity((float) value / 100);
    else if (which == m_ui->droplineOpacitySlider)
        sGraphProperty->setDroplineOpacity((float) value / 100);
    else if (which == m_ui->labelOpacitySlider)
        sGraphProperty->setCurveLabelOpacity((float) value / 100);

    applyToCurve();
}

void CurveSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->labelOffsetXSpinBox)
        sGraphProperty->setCurveLabelOffsetX(value);
    else if (which == m_ui->labelOffsetYSpinBox)
        sGraphProperty->setCurveLabelOffsetY(value);

    applyToCurve();
}

void CurveSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->symbolColorButton)
        sGraphProperty->setSymbolColor(which->color());
    else if (which == m_ui->symbolEdgeColorButton)
        sGraphProperty->setSymbolEdgeColor(which->color());
    else if (which == m_ui->droplineColorButton)
        sGraphProperty->setDroplineColor(which->color());
    else if (which == m_ui->labelColorButton)
        sGraphProperty->setCurveLabelColor(which->color());

    applyToCurve();
}

void CurveSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());

    if (which == m_ui->droplineShowHorizontalCheck)
        sGraphProperty->setHorizontalDroplineVisible(state == Qt::Checked);
    else if (which == m_ui->droplineShowVerticalCheck)
        sGraphProperty->setVerticalDroplineVisible(state == Qt::Checked);
    else if (which == m_ui->antialiasingCheck)
        sGraphProperty->setAntiAliasingCurve(state == Qt::Checked);

    applyToCurve();
}

void CurveSettings::onGroupBoxClicked(bool clicked)
{
    sGraphProperty->setCurveLabelVisible(clicked);
    applyToCurve();
}

void CurveSettings::onFontSelected(const QFont &f)
{
    GuiUtils::FontPushButton* which = qobject_cast<GuiUtils::FontPushButton*>(sender());

    if (which == m_ui->labelFontButton)
        sGraphProperty->setCurveLabelFont(f);
    applyToCurve();
}

void CurveSettings::initConns()
{
    connect(m_ui->lineWidthCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->lineOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));

    connect(m_ui->symbolColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->symbolEdgeColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->symbolCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->symbolSizeCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->symbolWidthCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->symbolOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));

    connect(m_ui->droplineOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->droplinePenStyleCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->droplineWidthCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(m_ui->droplineColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->droplineShowHorizontalCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->droplineShowVerticalCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->antialiasingCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(m_ui->labelGroupBox, SIGNAL(clicked(bool)), this, SLOT(onGroupBoxClicked(bool)));
    connect(m_ui->labelFontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));
    connect(m_ui->labelColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->labelOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(m_ui->labelPositionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->labelOffsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->labelOffsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
}

void CurveSettings::updateUi()
{
    m_ui->lineWidthCombo->setCurrentText(QString("%1").arg(sGraphProperty->curveLineWidth()));
    m_ui->lineOpacitySlider->setValue(sGraphProperty->curveLineOpacity() * 100);
    m_ui->antialiasingCheck->setChecked(sGraphProperty->isAntiAliasingCurve());

    m_ui->symbolCombo->setCurrentIndex(sGraphProperty->symbol() + 1);
    m_ui->symbolColorButton->setColor(sGraphProperty->symbolColor());
    m_ui->symbolEdgeColorButton->setColor(sGraphProperty->symbolEdgeColor());
    m_ui->symbolOpacitySlider->setValue(sGraphProperty->symbolOpacity() * 100);
    setComboBoxIndexByValue(m_ui->symbolSizeCombo, sGraphProperty->symbolSize());
    setComboBoxIndexByValue(m_ui->symbolWidthCombo, sGraphProperty->symbolWidth());

    m_ui->droplineShowHorizontalCheck->setChecked(sGraphProperty->isHorizontalDroplineVisible());
    m_ui->droplineShowVerticalCheck->setChecked(sGraphProperty->isVerticalDroplineVisible());
    m_ui->droplineColorButton->setColor(sGraphProperty->droplineColor());
    m_ui->droplineOpacitySlider->setValue(sGraphProperty->droplineOpacity() * 100);
    setComboBoxIndexByValue(m_ui->droplineWidthCombo, sGraphProperty->droplinePen().width());
    setComboBoxIndexByData<int>(m_ui->droplinePenStyleCombo, (int)sGraphProperty->droplinePen().style());

    m_ui->labelGroupBox->setChecked(sGraphProperty->isCurveLabelVisible());
    m_ui->labelFontButton->setFont(sGraphProperty->curveLabelFont());
    m_ui->labelColorButton->setColor(sGraphProperty->curveLabelColor());
    setComboBoxIndexByData<int>(m_ui->labelPositionCombo, sGraphProperty->curveLabelPosition());
    m_ui->labelOffsetXSpinBox->setValue(sGraphProperty->curveLabelOffsetX());
    m_ui->labelOffsetYSpinBox->setValue(sGraphProperty->curveLabelOffsetY());
    m_ui->labelOpacitySlider->setValue(sGraphProperty->curveLabelOpacity() * 100);
}

void CurveSettings::populateSymbolCombo()
{
    QComboBox* combo = m_ui->symbolCombo;
#define ADD_SYMBOL(MACRO, NAME) combo->addItem(QIcon(QwtSymbol(MACRO).pixmap()), NAME, MACRO);
    ADD_SYMBOL(QwtSymbol::NoSymbol, tr("No Symbol"))
    ADD_SYMBOL(QwtSymbol::Ellipse, tr("Ellipse"))
    ADD_SYMBOL(QwtSymbol::Rect, tr("Rect"))
    ADD_SYMBOL(QwtSymbol::Diamond, tr("Diamond"))
    ADD_SYMBOL(QwtSymbol::Triangle, tr("Triangle"))
    ADD_SYMBOL(QwtSymbol::DTriangle, tr("DTriangle"))
    ADD_SYMBOL(QwtSymbol::UTriangle, tr("UTriangle"))
    ADD_SYMBOL(QwtSymbol::LTriangle, tr("LTriangle"))
    ADD_SYMBOL(QwtSymbol::RTriangle, tr("RTriangle"))
    ADD_SYMBOL(QwtSymbol::Cross, tr("Cross"))
    ADD_SYMBOL(QwtSymbol::XCross, tr("XCross"))
    ADD_SYMBOL(QwtSymbol::HLine, tr("HLine"))
    ADD_SYMBOL(QwtSymbol::VLine, tr("VLine"))
    ADD_SYMBOL(QwtSymbol::Star1, tr("Star1"))
    ADD_SYMBOL(QwtSymbol::Star2, tr("Star2"))
    ADD_SYMBOL(QwtSymbol::Hexagon, tr("Hexagon"))
#undef ADD_SYMBOL
}

void CurveSettings::populateSizeCombo()
{
    for (int i = 3; i  < 12; ++i)
        m_ui->symbolSizeCombo->addItem(QString("%1").arg(i), i);
}

void CurveSettings::populateLabelPosCombo()
{
    m_ui->labelPositionCombo->addItem(tr("Center"), 0);
    m_ui->labelPositionCombo->addItem(tr("Left"), 1);
    m_ui->labelPositionCombo->addItem(tr("Right"), 2);
    m_ui->labelPositionCombo->addItem(tr("Above"), 3);
    m_ui->labelPositionCombo->addItem(tr("Below"), 4);
}
