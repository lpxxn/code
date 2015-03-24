#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace ComDataGui
{

static const QSize OriginalSize = QSize(350, 50);
static const QSize ExpandedSize = QSize(350, 100);

FilterDialog::FilterDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::FilterDialog)
{
    m_ui->setupUi(this);

    setWindowFlags(Qt::Tool);
    setWindowTitle(tr("Filter"));

    QValidator* validator = new QDoubleValidator(this);
    m_ui->filter1Edit->setValidator(validator);
    m_ui->filter2Edit->setValidator(validator);

    populateFilterCombo(m_ui->filter1Combo);
    populateFilterCombo(m_ui->filter2Combo);

    m_ui->andButton->setVisible(false);
    m_ui->orButton->setVisible(false);
    m_ui->filter2Combo->setVisible(false);
    m_ui->filter2Edit->setVisible(false);

    connect(m_ui->advancedButton, SIGNAL(clicked(bool)), this, SLOT(onAdvanedButtonClicked(bool)));
    connect(m_ui->filterButton, SIGNAL(clicked()), this, SLOT(onFilterButtonClicked()));
    connect(m_ui->filter1Combo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadFilter2Options(int)));

    setFixedSize(OriginalSize);
}

FilterDialog::~FilterDialog()
{

}

void FilterDialog::reset()
{
}

/*!
 * \reimp
 */
void FilterDialog::showEvent(QShowEvent *)
{
    m_ui->filter1Edit->setFocus();
}

void FilterDialog::onFilterButtonClicked()
{
    if (m_ui->advancedButton->isChecked()) {
        if (!m_ui->filter1Edit->text().isEmpty() && !m_ui->filter2Edit->text().isEmpty()) {
            FilterData fd;
            fd.type1 = m_ui->filter1Combo->currentIndex();
            fd.threshhold1 = m_ui->filter1Edit->text().toDouble();
            fd.type2 = m_ui->filter2Combo->currentIndex();
            fd.threshhold2 = m_ui->filter2Edit->text().toDouble();
            fd.op = m_ui->andButton->isChecked() ? FilterData::And : FilterData::Or;
            emit setFilter(fd);
        }
    } else {
        if (!m_ui->filter1Edit->text().isEmpty()) {
            FilterData fd;
            fd.type1 = m_ui->filter1Combo->currentIndex();
            fd.threshhold1 = m_ui->filter1Edit->text().toDouble();
            emit setFilter(fd);
        }
    }
    //    accept();
}

void FilterDialog::onAdvanedButtonClicked(bool checked)
{
    m_ui->andButton->setVisible(checked);
    m_ui->orButton->setVisible(checked);
    m_ui->filter2Combo->setVisible(checked);
    m_ui->filter2Edit->setVisible(checked);

    setFixedSize(checked ? ExpandedSize : OriginalSize);
}

void FilterDialog::loadFilter2Options(int index)
{

}

void FilterDialog::populateFilterCombo(QComboBox *cb)
{
    QStringList filters;
    filters << "<";
    filters << "<=";
    filters << ">";
    filters << ">=";
    filters << "=";
    filters << "!=";
#ifdef RESERVE
    filters << "Top";
    filters << "Bottom";
#endif
    cb->addItems(filters);
}

}
