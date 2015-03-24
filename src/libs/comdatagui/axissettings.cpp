#include "axissettings.h"
#include "ui_axissettings.h"
#include "constants.h"
#include "utils.h"

#include <comguiutils/guiconstants.h>
#include <comguiutils/colorpushbutton.h>
#include <comguiutils/fontpushbutton.h>
#include <comdatagui/curvegraph.h>
#include <comdatagui/graphproperty.h>

#include <QStackedLayout>

AxisSettings::AxisSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_AXIS);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));        
    setDisplayName(tr("Axis"));
}

AxisSettings::~AxisSettings()
{

}

/*!
 * \reimp
 */
QWidget *AxisSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::AxisSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    ComDataGui::Utils::populateSizeComboBox(m_ui->gridLineWidthCombo);
    ComDataGui::Utils::loadPenStyles(m_ui->gridLineTypeCombo);
    populateDisplayCombo();
    updateUi();
    initConns();
    return m_widget;
}

/*!
 * \reimp
 */
void AxisSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void AxisSettings::finish()
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
void AxisSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showCurve();
}

void AxisSettings::onColorChanged()
{
    GuiUtils::ColorPushButton* which = qobject_cast<GuiUtils::ColorPushButton*>(sender());

    if (which == m_ui->axisLabelColorButton)
        sGraphProperty->setAxisLabelColor(which->color());
    else if (which == m_ui->axisTickColorButton)
        sGraphProperty->setAxisTickColor(which->color());
    else if (which == m_ui->gridLineColorButton)
        sGraphProperty->setGridPenColor(which->color());

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void AxisSettings::onComboBoxIndexChanged(int index)
{
    QComboBox* which = qobject_cast<QComboBox*>(sender());

    if (which == m_ui->axisTickWidthCombo)
        sGraphProperty->setAxisTickWidth(index + 1);
    else if (which == m_ui->gridLineTypeCombo)
        sGraphProperty->setGridPenStyle(static_cast<Qt::PenStyle>(which->currentData().toInt()));
    else if (which == m_ui->gridLineWidthCombo)
        sGraphProperty->setGridPenWidth(which->currentText().toInt());
    else if (which == m_ui->axisTitleCombo)
        sGraphProperty->setAxisTitleType(index);
    else if (which == m_ui->notationCombo)
        sGraphProperty->setAxisNotation((QwtAbstractScaleDraw::Notation)index);

    makeDirty();
    VizGraphBaseSettings::apply();
}

void AxisSettings::onFontSelected(const QFont &f)
{
    GuiUtils::FontPushButton* which = qobject_cast<GuiUtils::FontPushButton*>(sender());

    if (which == m_ui->fontButton)
        sGraphProperty->setAxisLabelFont(f);

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void AxisSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());

    if (which == m_ui->gridMajorCheck) {
        sGraphProperty->setMajorGridVisible(state == Qt::Checked);
        if (state == Qt::Unchecked) {
            m_ui->gridMinorCheck->setChecked(false);
            m_ui->gridHorizontalCheck->setChecked(false);
            m_ui->gridVerticalCheck->setChecked(false);
        }
    } else if (which == m_ui->gridMinorCheck)
        sGraphProperty->setMinorGridVisible(state == Qt::Checked);
    else if (which == m_ui->gridHorizontalCheck)
        sGraphProperty->setHorizontalGridVisible(state == Qt::Checked);
    else if (which == m_ui->gridVerticalCheck)
        sGraphProperty->setVerticalGridVisible(state == Qt::Checked);
    else if (which == m_ui->showMajorLabelsCheck)
        sGraphProperty->setMajorLabelsVisible(state == Qt::Checked);
    else if (which == m_ui->showMinorLabelsCheck)
        sGraphProperty->setMinorLabelsVisible(state == Qt::Checked);
    else if (which == m_ui->showMinusSignCheck)
        sGraphProperty->setMinusSignVisible(state == Qt::Checked);
    else if (which == m_ui->showPlusSignCheck)
        sGraphProperty->setPlusSignVisible(state == Qt::Checked);

    makeDirty();
    VizGraphBaseSettings::apply();
}

void AxisSettings::onGroupBoxClicked(bool clicked)
{
    sGraphProperty->setGridVisible(clicked);
    makeDirty();
    VizGraphBaseSettings::apply();
}

void AxisSettings::onEditTextChanged(const QString &text)
{
    QLineEdit* which = qobject_cast<QLineEdit*>(sender());

    if (which == m_ui->prefixEdit)
        sGraphProperty->setPrefix(text);
    else if (which == m_ui->suffixEdit)
        sGraphProperty->setSuffix(text);

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void AxisSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->fieldWidthSpinBox)
        sGraphProperty->setDecimalWidth(value);

    makeDirty();
    VizGraphBaseSettings::apply();
}

/*!
 * \reimp
 */
void AxisSettings::updateUi()
{
    blockSignals(true);

    m_ui->axisLabelColorButton->setColor(sGraphProperty->axisLabelColor());
    m_ui->axisTickColorButton->setColor(sGraphProperty->axisTickColor());
    m_ui->axisTitleCombo->setCurrentIndex(sGraphProperty->axisTitleType());
    m_ui->fontButton->setFont(sGraphProperty->axisLabelFont());
    m_ui->axisTickWidthCombo->setCurrentIndex(sGraphProperty->axisTickWidth() - 1);
    m_ui->notationCombo->setCurrentIndex(sGraphProperty->axisNotation());
    m_ui->gridGroupBox->setChecked(sGraphProperty->isGridVisible());
    m_ui->gridHorizontalCheck->setChecked(sGraphProperty->isHorizontalGridVisible());
    m_ui->gridVerticalCheck->setChecked(sGraphProperty->isVerticalGridVisible());
    m_ui->gridMajorCheck->setChecked(sGraphProperty->isMajorGridVisible());
    m_ui->gridMinorCheck->setChecked(sGraphProperty->isMinorGridVisible());
    m_ui->gridLineColorButton->setColor(sGraphProperty->gridPenColor());
    m_ui->gridLineWidthCombo->setCurrentIndex(sGraphProperty->gridPenWidth() - 1);
    m_ui->gridLineTypeCombo->setCurrentIndex((int(sGraphProperty->gridPenStyle())) - 1);

    m_ui->showMajorLabelsCheck->setChecked(sGraphProperty->isMajorLabelsVisible());
    m_ui->showMinorLabelsCheck->setChecked(sGraphProperty->isMinorLabelsVisible());
    m_ui->fieldWidthSpinBox->setValue(sGraphProperty->decimalWidth());
    m_ui->prefixEdit->setText(sGraphProperty->prefix());
    m_ui->suffixEdit->setText(sGraphProperty->suffix());
    m_ui->showPlusSignCheck->setChecked(sGraphProperty->isPlusSignVisible());
    m_ui->showMinusSignCheck->setChecked(sGraphProperty->isMinusSignVisible());

    blockSignals(false);
}

/*!
 * \reimp
 */
void AxisSettings::initConns()
{
    connect(m_ui->axisLabelColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->axisTickColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->axisTickWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->notationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->axisTitleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->fontButton, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));

    connect(m_ui->gridHorizontalCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->gridVerticalCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->gridMajorCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->gridMinorCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->gridLineColorButton, SIGNAL(colorChanged()), this, SLOT(onColorChanged()));
    connect(m_ui->gridLineWidthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->gridLineTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(m_ui->gridGroupBox, SIGNAL(clicked(bool)), this, SLOT(onGroupBoxClicked(bool)));

    connect(m_ui->prefixEdit, SIGNAL(textChanged(QString)), this, SLOT(onEditTextChanged(QString)));
    connect(m_ui->suffixEdit, SIGNAL(textChanged(QString)), this, SLOT(onEditTextChanged(QString)));
    connect(m_ui->showMajorLabelsCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->showMinorLabelsCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->showPlusSignCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->showMinusSignCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->fieldWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
}

void AxisSettings::populateDisplayCombo()
{
    m_ui->notationCombo->addItem(tr("Decimal 1000"), 0);
//    m_ui->notationCombo->addItem(tr("Scientific 10^3"), 1);
//    m_ui->notationCombo->addItem(tr("Engineering 1k"), 2);
    m_ui->notationCombo->addItem(tr("Decimal 1,000"), 1);
    m_ui->notationCombo->addItem(tr("Scientific 1E3"), 2);
}

void AxisSettings::populateTitleCombo()
{
//    m_ui->titleCombo->addItem(tr("Long Name"));
//    m_ui->titleCombo->addItem(tr("Short Name"));
//    m_ui->titleCombo->addItem(tr("Unit"));
//    m_ui->titleCombo->addItem(tr("Long Name + Unit"));
//    m_ui->titleCombo->addItem(tr("Short Name + Unit"));
}
