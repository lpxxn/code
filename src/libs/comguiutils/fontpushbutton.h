#ifndef FONTPUSHBUTTON_H
#define FONTPUSHBUTTON_H

#include <QPushButton>

#include "comguiutils_global.h"

namespace GuiUtils
{

class COMGUIUTILS_EXPORT FontPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit FontPushButton(QWidget *parent = 0);
    explicit FontPushButton(const QString& text, QWidget* parent = 0);

    void setFont(const QFont &f);

signals:
    void fontSelected(const QFont& font);

private slots:
    void onClicked();

private:
    void init();

};

}
#endif // FONTPUSHBUTTON_H
