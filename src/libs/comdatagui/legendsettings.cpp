#include "legendsettings.h"
#include "ui_legendsettings.h"
#include "constants.h"

#include <comguiutils/guiconstants.h>
#include <comguiutils/colorpushbutton.h>
#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>

#include <qwt/qwt_plot_legenditem.h>

#include <QFontDialog>

LegendSettings::LegendSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_LEGEND);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("Legend"));
}

/*!
 * \reimp
 */
QWidget *LegendSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::LegendSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);

    populateModeCombo();
    populateHAlignCombo();
    populateVAlignCombo();
    populateLegendPosCombo();

    updateUi();
    initConns();

    return m_widget;
}

/*!
 * \reimp
 */
void LegendSettings::apply()
{
    if (!m_ui)
        return;

    sGraphProperty->setLegendVisible(m_ui->legendGroupBox->isChecked());

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void LegendSettings::finish()
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
void LegendSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showCurve();
}

void LegendSettings::onComboBoxCurrrentIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->legendItemHorizontalCombo)
        sGraphProperty->setLegendItemHAlign(which->currentData().toInt());
    else if (which == m_ui->legendItemVerticalCombo)
        sGraphProperty->setLegendItemVAlign(which->currentData().toInt());
    else if (which == m_ui->legendItemModeCombo)
        sGraphProperty->setLegendItemBgMode(which->currentData().toInt());
    else if (which == m_ui->legendPositionCombo)
        sGraphProperty->setLegendPosition(which->currentData().toInt());

    makeDirty();
    VizGraphBaseSettings::apply();
}

void LegendSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->legendColorButton)
        sGraphProperty->setLegendColor(which->color());
    else if (which == m_ui->legendItemBgColorButton)
        sGraphProperty->setLegendItemBgColor(which->color());
    else if (which == m_ui->borderColorButton)
        sGraphProperty->setLegendItemBorderColor(which->color());
    else if (which == m_ui->legendItemColorButton)
        sGraphProperty->setLegendItemColor(which->color());

    makeDirty();
    VizGraphBaseSettings::apply();
}

void LegendSettings::onFontSelected(const QFont &f)
{
    QPushButton* which = qobject_cast<QPushButton*>(sender());

    if (which == m_ui->legendFontButton)
        sGraphProperty->setLegendFont(f);
    else if (which == m_ui->legendItemFontButton)
        sGraphProperty->setLegendItemFont(f);

    makeDirty();
    VizGraphBaseSettings::apply();
}

void LegendSettings::onGroupBoxClicked(bool clicked)
{
    QGroupBox* which = qobject_cast<QGroupBox*>(sender());

    if (which == m_ui->legendGroupBox)
        sGraphProperty->setLegendVisible(clicked);
    else if (which == m_ui->legendItemGroupBox)
        sGraphProperty->setLegendItemVisible(clicked);

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void LegendSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->legendItemColumnSpinBox)
        sGraphProperty->setLegendItemColumns(value);

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void LegendSettings::initConns()
{
    connect(m_ui->legendColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->legendItemBgColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->borderColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->legendItemColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->legendFontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));
    connect(m_ui->legendItemFontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));

    connect(m_ui->legendItemHorizontalCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrrentIndexChanged(int)));
    connect(m_ui->legendItemVerticalCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrrentIndexChanged(int)));
    connect(m_ui->legendItemModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrrentIndexChanged(int)));
    connect(m_ui->legendPositionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrrentIndexChanged(int)));

    connect(m_ui->legendItemColumnSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));

    connect(m_ui->legendGroupBox, SIGNAL(clicked(bool)), this, SLOT(onGroupBoxClicked(bool)));
    connect(m_ui->legendItemGroupBox, SIGNAL(clicked(bool)), this, SLOT(onGroupBoxClicked(bool)));
}

/*!
 * \reimp
 */
void LegendSettings::updateUi()
{
    m_ui->legendGroupBox->setChecked(sGraphProperty->isLegendVisible());
    m_ui->legendItemGroupBox->setChecked(sGraphProperty->isLegendItemVisible());
    m_ui->legendFontButton->setFont(sGraphProperty->legendFont());
    m_ui->legendColorButton->setColor(sGraphProperty->legendColor());
    m_ui->legendPositionCombo->setCurrentIndex(sGraphProperty->legendPosition());
    m_ui->legendItemFontButton->setFont(sGraphProperty->legendItemFont());
    m_ui->legendItemBgColorButton->setColor(sGraphProperty->legendItemBgColor());
    m_ui->legendItemColorButton->setColor(sGraphProperty->legendItemColor());
    m_ui->borderColorButton->setColor(sGraphProperty->legendItemBorderColor());
    m_ui->legendItemColumnSpinBox->setValue(sGraphProperty->legendItemColumns());
    //    m_ui->legendItemVerticalCombo
    m_ui->legendItemModeCombo->setCurrentIndex(sGraphProperty->legendItemBgMode());

    switch (sGraphProperty->legendItemHAlign()) {
    case Qt::AlignLeft:
        m_ui->legendItemVerticalCombo->setCurrentIndex(0);
        break;

    case Qt::AlignHCenter:
        m_ui->legendItemVerticalCombo->setCurrentIndex(1);
        break;

    case Qt::AlignRight:
        m_ui->legendItemVerticalCombo->setCurrentIndex(2);
        break;
    }

    switch (sGraphProperty->legendItemVAlign()) {
    case Qt::AlignTop:
        m_ui->legendItemHorizontalCombo->setCurrentIndex(0);
        break;

    case Qt::AlignVCenter:
        m_ui->legendItemHorizontalCombo->setCurrentIndex(1);
        break;

    case Qt::AlignRight:
        m_ui->legendItemHorizontalCombo->setCurrentIndex(2);
        break;
    }
}

void LegendSettings::populateModeCombo()
{
    m_ui->legendItemModeCombo->addItem(tr("Legend"), QwtPlotLegendItem::LegendBackground);
    m_ui->legendItemModeCombo->addItem(tr("Item"), QwtPlotLegendItem::ItemBackground);
}

void LegendSettings::populateHAlignCombo()
{
    m_ui->legendItemHorizontalCombo->addItem(tr("Left"), Qt::AlignLeft);
    m_ui->legendItemHorizontalCombo->addItem(tr("Center"), Qt::AlignHCenter);
    m_ui->legendItemHorizontalCombo->addItem(tr("Right"), Qt::AlignRight);
}

void LegendSettings::populateVAlignCombo()
{
    m_ui->legendItemVerticalCombo->addItem(tr("Top"), Qt::AlignTop);
    m_ui->legendItemVerticalCombo->addItem(tr("Center"), Qt::AlignVCenter);
    m_ui->legendItemVerticalCombo->addItem(tr("Bottom"), Qt::AlignBottom);
}

void LegendSettings::populateLegendPosCombo()
{
    m_ui->legendPositionCombo->addItem(tr("Left"), QwtPlot::LeftLegend);
    m_ui->legendPositionCombo->addItem(tr("Right"), QwtPlot::RightLegend);
    m_ui->legendPositionCombo->addItem(tr("Bottom"), QwtPlot::BottomLegend);
    m_ui->legendPositionCombo->addItem(tr("Top"), QwtPlot::TopLegend);
}

