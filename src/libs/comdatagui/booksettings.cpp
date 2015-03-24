#include "booksettings.h"
#include "ui_booksettings.h"
#include "constants.h"

#include <comguiutils/fontpushbutton.h>
#include <comguiutils/colorpushbutton.h>
#include <comguiutils/guiconstants.h>
#include <comutils/settingsmanager.h>

#include <QSettings>
#include <QFontDatabase>

BookSettings::BookSettings(QObject *parent) :
    GuiUtils::IOptionsPage(parent)
{    
    setId(ComDataGui::Constants::SETTINGS_ID_BOOKS);
    setCategory(ComDataGui::Constants::SETTINGS_CATEGORY_BOOKS);
    setDisplayCategory(tr("Table"));
    setCategoryIcon(ComDataGui::Constants::SETTINGS_CATEGORY_BOOKS_ICON);
    setDisplayName(tr("Table"));
}

/*!
 * \reimp
 */
QWidget *BookSettings::createPage(QWidget *parent)
{
    m_ui = new Ui::BookSettings;
    m_widget = new QWidget(parent);
    m_ui->setupUi(m_widget);
    initConnection();
    initState();

    return m_widget;
}


/*!
 * \brief 初始化状态
 */
void BookSettings::initState()
{
    CommonUtils::SettingsManager* sm = CommonUtils::SettingsManager::instance();
    QSettings * setting =  sm->generalSettings();

    // Font
    m_font = setting->value(QStringLiteral("Table/Font"), QFont()).value<QFont>();
    QFont btnFont = m_font;
    btnFont.setPointSize(10);
    m_ui->btn_font->setFont(btnFont);
    m_ui->btn_font->setText(m_font.family());

    // Font color
    QColor fontColor = setting->value(QStringLiteral("Table/FontColor"), QColor(Qt::black)).value<QColor>();
    m_ui->btn_fontColor->setColor(fontColor);

    //data back Color
    QColor dataBackColor = setting->value(QStringLiteral("Table/DataBackColor"), QColor(Qt::transparent)).value<QColor>();
    m_ui->btn_DatabackgroundColor->setColor(dataBackColor);
    //rowNumber
    m_ui->sbox_Number->setValue(setting->value(QStringLiteral("Table/RowNumber"), 12).toInt());
    // display
    //
    char format =  setting->value(QStringLiteral("Table/Display"), 'g').toChar().toLatin1();
    int currentFormat = 0;
    if (format == 'e') {
        currentFormat = 1;
    } else if (format == 'f') {
        currentFormat = 2;
    }
    m_ui->cbo_display->setCurrentIndex(currentFormat);

    //Decimal Place
    m_ui->sbx_decimalPlace->setValue(setting->value(QStringLiteral("Table/DecimalPlace"), 5).toInt());

    //Column Width
    m_ui->sbx_columnWidth->setValue(setting->value(QStringLiteral("Table/ColumnWidth"), 100).toInt());

    // alternate color
    m_ui->cb_alternateColor->setChecked(setting->value(QStringLiteral("Table/AlterNate"), false).toBool());

    // checkbox ischecked
    m_ui->cb_LongName->setChecked(setting->value(QStringLiteral("Table/LongNameVisibility"), true).toBool());
    m_ui->cb_Unit->setChecked(setting->value(QStringLiteral("Table/UnitVisibility"), true).toBool());
    m_ui->cb_Comment->setChecked(setting->value(QStringLiteral("Table/CommentVisibility"), true).toBool());
    m_ui->cb_User->setChecked(setting->value(QStringLiteral("Table/UserVisibility"), true).toBool());
    m_ui->cb_Filter->setChecked(setting->value(QStringLiteral("Table/FilterVisibility"), false).toBool());
    m_ui->cb_Sparklines->setChecked(setting->value(QStringLiteral("Table/SparklinesVisibility"), false).toBool());

    // header background
    QColor headerBackColor = setting->value(QStringLiteral("Table/TableHeaderBackground"), QColor(Qt::yellow)).value<QColor>();
    m_ui->btn_HeaderColor->setColor(headerBackColor);

}

void BookSettings::initConnection()
{
    connect(m_ui->btn_font, SIGNAL(fontSelected(QFont)), this, SLOT(onFontSelected(QFont)));
}

/*!
 * \reimp
 */
void BookSettings::apply()
{
    if (!m_ui)
        return;

    CommonUtils::SettingsManager* sm = CommonUtils::SettingsManager::instance();
    QSettings * setting =  sm->generalSettings();

    setting->beginGroup(QStringLiteral("Table"));


    setting->setValue(QStringLiteral("Font"), m_font);
    setting->setValue(QStringLiteral("FontColor"), m_ui->btn_fontColor->currentColor());
    setting->setValue(QStringLiteral("DataBackColor"), m_ui->btn_DatabackgroundColor->currentColor());
    setting->setValue(QStringLiteral("RowNumber"), m_ui->sbox_Number->value());

    QChar format = 'g';
    if (m_ui->cbo_display->currentIndex() == 1) {
        format = 'e';
    } else if (m_ui->cbo_display->currentIndex() == 2) {
        format = 'f';
    }

    setting->setValue(QStringLiteral("Display"), format);

    setting->setValue(QStringLiteral("DecimalPlace"), m_ui->sbx_decimalPlace->value());
    setting->setValue(QStringLiteral("ColumnWidth"), m_ui->sbx_columnWidth->value());
    setting->setValue(QStringLiteral("AlterNate"), m_ui->cb_alternateColor->isChecked());
    setting->setValue(QStringLiteral("CommentVisibility"), m_ui->cb_Comment->isChecked());
    setting->setValue(QStringLiteral("FilterVisibility"), m_ui->cb_Filter->isChecked());
    setting->setValue(QStringLiteral("LongNameVisibility"), m_ui->cb_LongName->isChecked());
    setting->setValue(QStringLiteral("SparklinesVisibility"), m_ui->cb_Sparklines->isChecked());
    setting->setValue(QStringLiteral("UnitVisibility"), m_ui->cb_Unit->isChecked());
    setting->setValue(QStringLiteral("UserVisibility"), m_ui->cb_User->isChecked());

    setting->setValue(QStringLiteral("TableHeaderBackground"), m_ui->btn_HeaderColor->currentColor());
    setting->endGroup();

}

/*!
 * \reimp
 */
void BookSettings::finish()
{
    if (!m_ui) // page was never shown
        return;
    delete m_ui;
    m_ui = nullptr;
}

void BookSettings::onFontSelected(const QFont &font)
{
    m_font = font;    
}



