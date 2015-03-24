#include "vizgraphbasesettings.h"
#include "settingkeys.h"

#include <comdatagui/curvegraph.h>
#include <comdatagui/pie2dgraph.h>
#include <comdatagui/graphproperty.h>
#include <comdatagui/barchartgraph.h>
#include <comdatagui/bubblegraph.h>
#include <comutils/settingsmanager.h>

#include <QComboBox>
#include <QFontDatabase>
#include <QListWidget>
#include <QSettings>
#include <QStackedWidget>

// the declarations and initializations of the static variables matter
ComDataGui::CurveGraph* VizGraphBaseSettings::sCurve = nullptr;
ComDataGui::BarChartGraph* VizGraphBaseSettings::sBarChart = nullptr;
ComDataGui::Pie2DGraph* VizGraphBaseSettings::sPie2D = nullptr;
ComDataGui::BubbleGraph* VizGraphBaseSettings::sBubble = nullptr;
ComDataGui::GraphProperty* VizGraphBaseSettings::sGraphProperty = nullptr;
QStackedWidget* VizGraphBaseSettings::sDemoWidget = nullptr;

bool VizGraphBaseSettings::sIsDirty = true;
bool VizGraphBaseSettings::sIsWritten = false;

VizGraphBaseSettings::VizGraphBaseSettings()
{
    init();
}

void VizGraphBaseSettings::apply()
{
    if (sIsDirty) {
        qDebug() << __FUNCTION__;
        applyToBubble();
        applyToCurve();
        applyToBarChart();
        applyToPie();
        sIsDirty = false;
    }
}

void VizGraphBaseSettings::applyToBubble()
{
    bubble()->setProperty(*sGraphProperty);
}

void VizGraphBaseSettings::applyToCurve()
{
    curve()->setProperty(*sGraphProperty);
}

void VizGraphBaseSettings::applyToBarChart()
{
    barChart()->setProperty(*sGraphProperty);
}

void VizGraphBaseSettings::applyToPie()
{
    return;
    pie()->setProperty(*sGraphProperty);
}

ComDataGui::CurveGraph* VizGraphBaseSettings::curve()
{
    if (!sCurve) {
        sCurve = new ComDataGui::CurveGraph(ComDataGui::CurveGraph::Line);

        qsrand(QTime::currentTime().msecsSinceStartOfDay());
        QList<QSharedPointer<ComData::ColumnBase>> columns;
        ComData::IntegerColumn* ic = new ComData::IntegerColumn();
        ic->setSignificance(ComData::ColumnBase::X);
        ic->setName("X Values");
        ic->setLongName("Long X Values");
        ic->setUnit("ms");
        ComData::IntegerColumn* ic2 = new ComData::IntegerColumn();
        ic2->setSignificance(ComData::ColumnBase::Y);
        ic2->setName("Y Values");
        ic2->setLongName("Long Y Values");
        ic2->setUnit("count");
        ComData::IntegerColumn* ic3 = new ComData::IntegerColumn();
        ic3->setSignificance(ComData::ColumnBase::Y);
        ic3->setName("Y Values");
        ic3->setLongName("Long Y Values");
        ic3->setUnit("count");

        ComData::IntegerColumn* ic4 = new ComData::IntegerColumn();
        ic4->setSignificance(ComData::ColumnBase::Y);
        ic4->setName("Y Values");
        ic4->setLongName("Long Y Values");
        ic4->setUnit("count");

        ComData::IntegerColumn* xErrors = new ComData::IntegerColumn();
        xErrors->setSignificance(ComData::ColumnBase::XError);
        ComData::IntegerColumn* yErrors = new ComData::IntegerColumn();
        yErrors->setSignificance(ComData::ColumnBase::YError);

        for (int i = 0; i < 5; ++i) {
            ic->setCell(i, i);
            ic2->setCell(i, rand() % 100);
            ic3->setCell(i, rand() % 200);
            ic4->setCell(i, rand() % 300);
            xErrors->setCell(i, 5);
            yErrors->setCell(i, 5);
        }

        columns.append(QSharedPointer<ComData::ColumnBase>(ic));
        columns.append(QSharedPointer<ComData::ColumnBase>(ic2));
        columns.append(QSharedPointer<ComData::ColumnBase>(ic3));
        columns.append(QSharedPointer<ComData::ColumnBase>(ic4));
        columns.append(QSharedPointer<ComData::ColumnBase>(xErrors));
        columns.append(QSharedPointer<ComData::ColumnBase>(yErrors));
        sCurve->setData(columns);
        sCurve->initialize();
    }

    return sCurve;
}

ComDataGui::BarChartGraph* VizGraphBaseSettings::barChart()
{
    if (!sBarChart) {
        sBarChart = new ComDataGui::BarChartGraph(ComDataGui::BarChartGraph::ColumnPlusLabel);

        ComData::IntegerColumn* col = new ComData::IntegerColumn();
        col->setSignificance(ComData::ColumnBase::X);
        col->setName("X Values");
        col->setLongName("X Values (BarChart)");
        col->setUnit("m");
        ComData::IntegerColumn* col2 = new ComData::IntegerColumn();
        col2->setSignificance(ComData::ColumnBase::Y);
        col2->setName("Y Values");
        col2->setLongName("Y Values (BarChart)");
        col2->setUnit("month");
        for (int i = 0; i < 5; ++i) {
            col->append(i);
            col2->append(rand() % 100);
        }

        QList<QSharedPointer<ComData::ColumnBase>> data;
        data.append(QSharedPointer<ComData::ColumnBase>(col));
        data.append(QSharedPointer<ComData::ColumnBase>(col2));

        sBarChart->setData(data);
        sBarChart->initialize();
    }
    return sBarChart;
}

ComDataGui::Pie2DGraph* VizGraphBaseSettings::pie()
{
    if (!sPie2D) {
        sPie2D = new ComDataGui::Pie2DGraph;

        ComData::IntegerColumn* col = new ComData::IntegerColumn();
        col->setSignificance(ComData::ColumnBase::X);
        for (int i = 0; i < 5; ++i)
            col->append(rand() % 100);

        QList<QSharedPointer<ComData::ColumnBase>> data;
        data.append(QSharedPointer<ComData::ColumnBase>(col));

        sPie2D->setData(data);
        sPie2D->initialize();
    }

    return sPie2D;
}

ComDataGui::BubbleGraph* VizGraphBaseSettings::bubble()
{
    if (!sBubble) {
        sBubble = new ComDataGui::BubbleGraph();
        ComData::IntegerColumn* col = new ComData::IntegerColumn();
        col->setSignificance(ComData::ColumnBase::X);
        ComData::IntegerColumn* col2 = new ComData::IntegerColumn();
        col2->setSignificance(ComData::ColumnBase::Y);
        ComData::IntegerColumn* col3 = new ComData::IntegerColumn();
        col3->setSignificance(ComData::ColumnBase::Y);
        for (int i = 0; i < 5; ++i) {
            col->append(i);
            col2->append(rand() % 100);
            col3->append(rand() % 100);
        }

        QList<QSharedPointer<ComData::ColumnBase>> data;
        data.append(QSharedPointer<ComData::ColumnBase>(col));
        data.append(QSharedPointer<ComData::ColumnBase>(col2));
        data.append(QSharedPointer<ComData::ColumnBase>(col3));

        sBubble->setData(data);
        sBubble->initialize();
    }

    return sBubble;
}

void VizGraphBaseSettings::populateColorComboBox(QComboBox *cb)
{
    const QStringList colorNames = QColor::colorNames();
    int index = 0;
    foreach (const QString &colorName, colorNames) {
        const QColor color(colorName);
        cb->addItem(colorName, color);
        const QModelIndex idx = cb->model()->index(index++, 0);
        cb->model()->setData(idx, color, Qt::BackgroundColorRole);
    }
}

void VizGraphBaseSettings::populateFontComboBox(QComboBox* cb)
{
    QFontDatabase fd;
    cb->addItems(fd.families());
}

void VizGraphBaseSettings::populateFontSizeComboBox(QComboBox* cb)
{
    QList<int> fontSizes = QFontDatabase::standardSizes();
    QStringList strSizes;
    for (int size : fontSizes)
        strSizes.append(QString("%1").arg(size));
    cb->addItems(strSizes);
}

void VizGraphBaseSettings::populateColorList(QListWidget *lw)
{
    const QStringList colorNames = QColor::colorNames();
    foreach (const QString &colorName, colorNames) {
        const QColor color(colorName);
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(colorName);
        item->setBackgroundColor(color);
        lw->addItem(item);
    }
}

void VizGraphBaseSettings::setComboBoxIndexByValue(QComboBox *cb, int value)
{
    for (int i = 0; i < cb->count(); ++i) {
        if (cb->itemText(i).toInt() == value) {
            cb->setCurrentIndex(i);
            return;
        }
    }
}

void VizGraphBaseSettings::init()
{
    sIsWritten = false;
    if (sGraphProperty == nullptr) {
        qDebug() << "creating new GraphProperty instance";
        sGraphProperty = new ComDataGui::GraphProperty();
    }

    if (sDemoWidget == nullptr) {
        sDemoWidget = new QStackedWidget();
        sDemoWidget->addWidget(curve());
        sDemoWidget->addWidget(pie());
        sDemoWidget->addWidget(barChart());
        sDemoWidget->addWidget(bubble());
    }

    readSettings();
    apply();
}

void VizGraphBaseSettings::finalize()
{
    writeSettings();
    release();
}

template <class T>
inline void cleanup(T** obj)
{
    delete *obj;
    *obj = nullptr;
}

void VizGraphBaseSettings::release()
{
    cleanup(&sCurve);
    cleanup(&sBarChart);
    cleanup(&sPie2D);
    cleanup(&sBubble);
    cleanup(&sGraphProperty);
    cleanup(&sDemoWidget);
}

void VizGraphBaseSettings::showCurve()
{
    sDemoWidget->show();
    sDemoWidget->setCurrentWidget((QWidget*)curve());
}

void VizGraphBaseSettings::showPie()
{
    sDemoWidget->show();
    sDemoWidget->setCurrentWidget((QWidget*)pie());
}

void VizGraphBaseSettings::showBarChart()
{
    sDemoWidget->show();
    sDemoWidget->setCurrentWidget((QWidget*)barChart());
}

void VizGraphBaseSettings::showBubble()
{
    sDemoWidget->show();
    sDemoWidget->setCurrentWidget((QWidget*)bubble());
}

void VizGraphBaseSettings::readSettings()
{
    if (!sGraphProperty)
        return;
    sGraphProperty->readSettings();
}

void VizGraphBaseSettings::writeSettings()
{
    if (!sGraphProperty)
        return;

    if (!sIsWritten) {
        sGraphProperty->writeSettings();
        sIsWritten = true;
    } else {
        qDebug() << __FUNCTION__ << ": finished writting records, quit.";
    }
}

void VizGraphBaseSettings::makeDirty()
{
    sIsDirty = true;
}
