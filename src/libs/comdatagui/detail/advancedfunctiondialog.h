#ifndef ADVANCEDFUNCTIONDIALOG
#define ADVANCEDFUNCTIONDIALOG

#include <QDialog>

class QAbstractButton;
class QListWidgetItem;

namespace Ui
{
    class AdvancedFunctionDialog;
}

namespace ComDataGui
{

/*!
 * \class 用户函数对话框类.
 */
class AdvancedFunctionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdvancedFunctionDialog(QWidget* parent = 0);
    ~AdvancedFunctionDialog();

    void setColumnList(const QStringList& list);
    void setText(const QString& text);

signals:
    void setExpression(const QString& exp);

protected:
    void showEvent(QShowEvent *);

private slots:
    void onAccepted();
    void onBoxButtonClicked(QAbstractButton* button);
    void onFuncTypeComboIndexChanged(int index);
    void onButtonPressed();
    void onFuncListDoubleClicked(QListWidgetItem* item);
    void onColumnListDoubleClicked(QListWidgetItem* item);

private:
    void populateMathFuncs();
    void populateTriangularFuncs();
    void appendText(const QString& text);
    void setCursorToParenthesis();

private:
    Ui::AdvancedFunctionDialog* m_ui;
};

}
#endif // ADVANCEDFUNCTIONDIALOG

