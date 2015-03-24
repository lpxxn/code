#ifndef BOOKSETTINGS_H
#define BOOKSETTINGS_H

#include <comguiutils/ioptionspage.h>
#include "comdatagui_global.h"
#include <QFont>

namespace Ui {
    class BookSettings;
}

class COMDATAGUI_EXPORT BookSettings : public GuiUtils::IOptionsPage
{
    Q_OBJECT
public:
    explicit BookSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);    
    void initState();    
    void initConnection();
    void apply();
    void finish();

private slots:    
    void onFontSelected(const QFont& font);

private:
    Ui::BookSettings *m_ui = nullptr;
    QWidget* m_widget;
    QFont m_font;
};

#endif // BOOKSETTINGS_H
