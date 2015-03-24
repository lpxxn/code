#ifndef FILTERDIALOG
#define FILTERDIALOG

#include <QDialog>

#include "complextableview.h"

class QComboBox;

namespace Ui
{
    class FilterDialog;
}

namespace ComDataGui
{

/*!
 * \class 过滤器对话框类.
 */
class FilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterDialog(QWidget* parent = 0);
    ~FilterDialog();

    void reset();

protected:
    void showEvent(QShowEvent *);

private slots:
    void onFilterButtonClicked();
    void onAdvanedButtonClicked(bool checked);
    void loadFilter2Options(int index);

signals:
    void setFilter(const FilterData& fd);

private:
    void populateFilterCombo(QComboBox* cb);

private:
    Ui::FilterDialog* m_ui = nullptr;
};

}
#endif // FILTERDIALOG

