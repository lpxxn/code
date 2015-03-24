#include "multiitemsettings.h"
#include "ui_generalsettings.h"
#include "constants.h"
#include "utils.h"
#include "symbolcombobox.h"

#include <comguiutils/colorcombobox.h>
#include <comdatagui/graphproperty.h>

#include <QStackedLayout>

using namespace ComDataGui;

MultiItemSettings::MultiItemSettings(QObject *parent) :
    SettingsBase(parent)
{
    setId(ComDataGui::Constants::SETTINGS_ID_MULTIITEM);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_VIZGRAPH);
    setDisplayCategory(tr("2D Plots"));
    setDisplayName(tr("MultiItem"));
}

/*!
 * \reimp
 */
QWidget *MultiItemSettings::createPage(QWidget *parent)
{
    m_ui = new Ui::GeneralSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    populateLineColorTable();
    populatePatternColorTable();
    populateLineStyleTable();
    populateBubbleStyleTable();
    updateUi();
    initConns();
    return m_widget;
}

/*!
 * \reimp
 */
void MultiItemSettings::apply()
{
    if (!m_ui)
        return;

    VizGraphBaseSettings::apply();
    VizGraphBaseSettings::writeSettings();
}

/*!
 * \reimp
 */
void MultiItemSettings::finish()
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
void MultiItemSettings::onActivate()
{
    m_ui->demoLayout->addWidget((QWidget*)sDemoWidget);
}

/*!
 * \reimp
 */
void MultiItemSettings::onCheckBoxStateChanged(int state)
{
    QCheckBox* which = qobject_cast<QCheckBox*>(sender());
    const bool use = state == Qt::Checked;

    if (which == m_ui->lineColorCheck) {
        sGraphProperty->setUseGroupLineColor(use);
        if (use)
            updateLineColors();

        applyToCurve();
    } else if (which == m_ui->patternColorCheck) {
        sGraphProperty->setUseGroupPatternColor(use);
        if (use)
            updatePatternColors();

        applyToBubble();
        applyToBarChart();
        applyToPie();

    } else if (which == m_ui->lineStyleCheck) {
        if (use)
            updateLineStyles();
        sGraphProperty->setUseGroupPenStyle(use);

        applyToCurve();
    } else if (which == m_ui->bubbleStyleCheck) {
        if (use)
            updateBubbleStyles();

        sGraphProperty->setUseGroupBubbleStyle(use);
        applyToBubble();
    }
}

/*!
 * \reimp
 */
void MultiItemSettings::onComboBoxIndexChanged(int index)
{
    Q_UNUSED(index)
    updateBubbleStyles();
    applyToBubble();
}

void MultiItemSettings::onLineColorTableEntered()
{
    showCurve();
}

void MultiItemSettings::onPatternColorTableEntered()
{
    showBarChart();
}

void MultiItemSettings::onPenStyleTableEntered()
{
    showCurve();
}

void MultiItemSettings::onBubbleStyleTableEntered()
{
    showBubble();
}

void MultiItemSettings::onLineColorComboIndexChanged(int index)
{
    Q_UNUSED(index)
    updateLineColors();
    applyToCurve();
}

void MultiItemSettings::onPatternColorComboIndexChanged(int index)
{
    Q_UNUSED(index)
    updatePatternColors();

    applyToBarChart();
    applyToBubble();
    applyToPie();
}

void MultiItemSettings::onLineStyleComboIndexChanged(int index)
{
    Q_UNUSED(index)
    updateLineStyles();
    applyToCurve();
}

/*!
 * \reimp
 */
void MultiItemSettings::initConns()
{
    ComDataGui::Utils::EnterNotification* lineTableEnterNof = new ComDataGui::Utils::EnterNotification(m_ui->lineColorTable);
    m_ui->lineColorTable->installEventFilter(lineTableEnterNof);
    connect(lineTableEnterNof, SIGNAL(entered()), this, SLOT(onLineColorTableEntered()));

    ComDataGui::Utils::EnterNotification* patternTableEnterNof = new ComDataGui::Utils::EnterNotification(m_ui->patternColorTable);
    m_ui->patternColorTable->installEventFilter(patternTableEnterNof);
    connect(patternTableEnterNof, SIGNAL(entered()), this, SLOT(onPatternColorTableEntered()));

    ComDataGui::Utils::EnterNotification* lineStyleEnterNof = new ComDataGui::Utils::EnterNotification(m_ui->lineStyleTable);
    m_ui->lineStyleTable->installEventFilter(lineStyleEnterNof);
    connect(lineStyleEnterNof, SIGNAL(entered()), this, SLOT(onPenStyleTableEntered()));

    ComDataGui::Utils::EnterNotification* bubbleTableEnterNof = new ComDataGui::Utils::EnterNotification(m_ui->bubbleStyleTable);
    m_ui->bubbleStyleTable->installEventFilter(bubbleTableEnterNof);
    connect(bubbleTableEnterNof, SIGNAL(entered()), this, SLOT(onBubbleStyleTableEntered()));

    connect(m_ui->lineColorCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->patternColorCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->lineStyleCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_ui->bubbleStyleCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
}

/*!
 * \reimp
 */
void MultiItemSettings::updateUi()
{
    m_ui->lineColorCheck->setChecked(sGraphProperty->useGroupLineColor());
    m_ui->patternColorCheck->setChecked(sGraphProperty->useGroupPatternColor());
    m_ui->lineStyleCheck->setChecked(sGraphProperty->useGroupPenStyle());
    m_ui->bubbleStyleCheck->setChecked(sGraphProperty->useGroupBubbleStyle());

    QMap<QString, QVariant> lineClrsMap = sGraphProperty->groupLineColors();
    if (!lineClrsMap.isEmpty()) {
        for (int i = 0; i < m_ui->lineColorTable->rowCount(); ++i) {
            GuiUtils::ColorComboBox* cb = qobject_cast<GuiUtils::ColorComboBox*>
                    (m_ui->lineColorTable->cellWidget(i, 0));
            cb->blockSignals(true);
            cb->setColor(qvariant_cast<QColor>(lineClrsMap.value(QString("%1").arg(i))));
            cb->blockSignals(false);
        }
    }

    QMap<QString, QVariant> patternClrsMap = sGraphProperty->groupPatternColors();
    if (!patternClrsMap.isEmpty()) {
        for (int i = 0; i < m_ui->patternColorTable->rowCount(); ++i) {
            GuiUtils::ColorComboBox* cb = qobject_cast<GuiUtils::ColorComboBox*>
                    (m_ui->patternColorTable->cellWidget(i, 0));
            cb->blockSignals(true);
            cb->setColor(qvariant_cast<QColor>(patternClrsMap.value(QString("%1").arg(i))));
            cb->blockSignals(false);
        }
    }

    m_ui->lineStyleTable->blockSignals(true);
    QMap<QString, QVariant> lineStylesMap = sGraphProperty->groupPenStyle();
    if (!lineStylesMap.isEmpty()) {
        for (int i = 0; i < m_ui->lineStyleTable->rowCount(); ++i) {
            QComboBox* cb = qobject_cast<QComboBox*>(m_ui->lineStyleTable->cellWidget(i, 0));
            cb->blockSignals(true);
            setComboBoxIndexByData<int>(cb, lineStylesMap.value(QString("%1").arg(i)).toInt());
            cb->blockSignals(false);
        }
    }
    m_ui->lineStyleTable->blockSignals(false);

    QMap<QString, QVariant> bubbleStylesMap = sGraphProperty->groupBubbleStyles();
    if (!bubbleStylesMap.isEmpty()) {
        for (int i = 0; i < m_ui->lineStyleTable->rowCount(); ++i) {
            SymbolComboBox* cb = qobject_cast<SymbolComboBox*>
                    (m_ui->bubbleStyleTable->cellWidget(i, 0));
            cb->blockSignals(true);
            cb->setStyle(static_cast<QwtSymbol::Style>(bubbleStylesMap.value(QString("%1").arg(i)).toInt()));
            cb->blockSignals(false);
        }
    }

    m_ui->lineColorTable->setEnabled(sGraphProperty->useGroupLineColor());
    m_ui->patternColorTable->setEnabled(sGraphProperty->useGroupPatternColor());
    m_ui->lineStyleTable->setEnabled(sGraphProperty->useGroupPenStyle());
    m_ui->bubbleStyleTable->setEnabled(sGraphProperty->useGroupBubbleStyle());
}

/*!
 * \brief 填充线型颜色列表.
 */
void MultiItemSettings::populateLineColorTable()
{
    populateColorTable(m_ui->lineColorTable, SLOT(onLineColorComboIndexChanged(int)));
}

/*!
 * \brief 填充填充图形颜色列表.
 */
void MultiItemSettings::populatePatternColorTable()
{
    populateColorTable(m_ui->patternColorTable, SLOT(onPatternColorComboIndexChanged(int)));
}

/*!
 * \brief 填充线型样式列表.
 */
void MultiItemSettings::populateLineStyleTable()
{
    m_ui->lineStyleTable->clearContents();
    m_ui->lineStyleTable->setAlternatingRowColors(true);
    m_ui->lineStyleTable->setColumnCount(1);
    m_ui->lineStyleTable->horizontalHeader()->setStretchLastSection(true);
    m_ui->lineStyleTable->setHorizontalHeaderLabels(QStringList() << tr("Choose pen style"));

    const int penCount = GuiUtils::ColorComboBox::defaultColors().size();
    m_ui->lineStyleTable->setRowCount(penCount);
    for (int i = 0; i < penCount; ++i) {
        QComboBox* cb = new QComboBox;
        ComDataGui::Utils::loadPenStyles(cb);
        m_ui->lineStyleTable->setCellWidget(i, 0, cb);
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineStyleComboIndexChanged(int)));
    }
}

/*!
 * \brief 填充冒泡图样式列表.
 */
void MultiItemSettings::populateBubbleStyleTable()
{
    m_ui->bubbleStyleTable->clearContents();
    m_ui->bubbleStyleTable->setAlternatingRowColors(true);
    m_ui->bubbleStyleTable->setColumnCount(1);
    m_ui->bubbleStyleTable->horizontalHeader()->setStretchLastSection(true);
    m_ui->bubbleStyleTable->setHorizontalHeaderLabels(QStringList() << tr("Choose bubble style"));

    const int penCount = GuiUtils::ColorComboBox::defaultColors().size();
    m_ui->bubbleStyleTable->setRowCount(penCount);

    for (int i = 0; i < penCount; ++i) {
        SymbolComboBox* cb = new SymbolComboBox;
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
        m_ui->bubbleStyleTable->setCellWidget(i, 0, cb);
    }
}

/*!
 * \brief 填充颜色列表.
 * \param table 表格控件
 * \param slot 槽函数
 */
void MultiItemSettings::populateColorTable(QTableWidget *table, const char* slot)
{
    table->clearContents();
    table->setAlternatingRowColors(true);
    table->setColumnCount(1);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setHorizontalHeaderLabels(QStringList() << tr("Choose colors"));
    QList<QColor> clrList = GuiUtils::ColorComboBox::colorList();
    table->setRowCount(clrList.size());
    for (int i = 0; i < clrList.size(); ++i) {
        GuiUtils::ColorComboBox* cb = new GuiUtils::ColorComboBox;
        table->setCellWidget(i, 0, cb);
        connect(cb, SIGNAL(currentIndexChanged(int)), this, slot);
    }
}

/*!
 * \brief 更新线型样式.
 */
void MultiItemSettings::updateLineStyles()
{
    QMap<QString, QVariant> styles;
    for (int i = 0; i < m_ui->lineStyleTable->rowCount(); ++i) {
        QComboBox* cb = qobject_cast<QComboBox*>(m_ui->lineStyleTable->cellWidget(i, 0));
        styles.insert(QString("%1").arg(i), cb->currentData().toInt());
    }

    sGraphProperty->setGroupPenStyles(styles);
}

/*!
 * \brief 更新线型颜色.
 */
void MultiItemSettings::updateLineColors()
{
    QMap<QString, QVariant> clrs;
    for (int i = 0; i < m_ui->lineColorTable->rowCount(); ++i) {
        GuiUtils::ColorComboBox* cb = qobject_cast<GuiUtils::ColorComboBox*>(m_ui->lineColorTable->cellWidget(i, 0));
        clrs.insert(QString("%1").arg(i), cb->color());
    }

    sGraphProperty->setGroupLineColors(clrs);
}

/*!
 * \brief 更新填充图形颜色.
 */
void MultiItemSettings::updatePatternColors()
{
    QMap<QString, QVariant> clrs;
    for (int i = 0; i < m_ui->patternColorTable->rowCount(); ++i) {
        GuiUtils::ColorComboBox* cb = qobject_cast<GuiUtils::ColorComboBox*>(m_ui->patternColorTable->cellWidget(i, 0));
        clrs.insert(QString("%1").arg(i), cb->color());
    }

    sGraphProperty->setGroupPatternColors(clrs);
}

/*!
 * \brief 更新冒泡图样式.
 */
void MultiItemSettings::updateBubbleStyles()
{
    QMap<QString, QVariant> styles;

    for (int i = 0; i < m_ui->bubbleStyleTable->rowCount(); ++i) {
        SymbolComboBox* cb = qobject_cast<SymbolComboBox*>(m_ui->bubbleStyleTable->cellWidget(i, 0));
        styles.insert(QString("%1").arg(i), cb->selectedSymbol());
    }

    sGraphProperty->setGroupBubbleStyles(styles);
}
