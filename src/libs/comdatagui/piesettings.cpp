#include "piesettings.h"
#include "ui_piesettings.h"
#include "constants.h"

#include <comguiutils/guiconstants.h>
#include <comdatagui/curvegraph.h>

PieSettings::PieSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_PIES);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setCategoryIcon(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH_ICON);
    setDisplayName(tr("Pies"));
}

QWidget *PieSettings::createPage(QWidget *parent)
{
    init();
    m_ui = new Ui::PieSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);

    updateUi();
    initConns();

    return m_widget;
}

void PieSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

void PieSettings::finish()
{
    if (!m_ui) // page was never shown
        return;
    delete m_ui;
    m_ui = nullptr;

    release();
}

void PieSettings::onActivate()
{
    if (m_ui)
        m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
    showPie();
}

void PieSettings::onSpinBoxValueChanged(int value)
{
    QSpinBox* which = qobject_cast<QSpinBox*>(sender());

    if (which == m_ui->startAngleSpinBox)
        sGraphProperty->setPieStartAngle(value);
    else if (which == m_ui->thicknessSpinBox)
        sGraphProperty->setPieThickness(value);
    else if (which == m_ui->viewAngleSpinBox)
        sGraphProperty->setPieViewAngle(value);
    else if (which == m_ui->radiusRatioSpinBox)
        sGraphProperty->setPieRadiusRatio((float) value / 100);
    else if (which == m_ui->displacementRatioSpinBox)
        sGraphProperty->setPieDisplacementRatio((float) value / 100);

    applyToPie();
}

void PieSettings::onCheckBoxStateChanged(int state)
{
    sGraphProperty->setPieCounterClockwise(state == Qt::Checked);
    applyToPie();
}

void PieSettings::updateUi()
{
    m_ui->radiusRatioSpinBox->setValue(sGraphProperty->pieRadiusRatio() * 100);
    m_ui->displacementRatioSpinBox->setValue(sGraphProperty->pieDisplacementRatio() * 100);
    m_ui->viewAngleSpinBox->setValue(sGraphProperty->pieViewAngle());
    m_ui->startAngleSpinBox->setValue(sGraphProperty->pieStartAngle());
    m_ui->thicknessSpinBox->setValue(sGraphProperty->pieThickness());
    m_ui->counterClockwiseCheck->setChecked(sGraphProperty->isPieCounterClockwise());
}

void PieSettings::initConns()
{
    connect(m_ui->counterClockwiseCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(m_ui->thicknessSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->startAngleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->viewAngleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->radiusRatioSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(m_ui->displacementRatioSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
}
