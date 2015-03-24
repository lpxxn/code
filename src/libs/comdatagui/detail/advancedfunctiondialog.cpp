#include "advancedfunctiondialog.h"
#include "ui_advancedfunctiondialog.h"

namespace ComDataGui
{

AdvancedFunctionDialog::AdvancedFunctionDialog(QWidget *parent): QDialog(parent), m_ui(new Ui::AdvancedFunctionDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onBoxButtonClicked(QAbstractButton*)));
    connect(m_ui->funcTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onFuncTypeComboIndexChanged(int)));
    connect(m_ui->columnList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onColumnListDoubleClicked(QListWidgetItem*)));
    connect(m_ui->funcList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onFuncListDoubleClicked(QListWidgetItem*)));

    connect(m_ui->sinButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->cosButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->piButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->eButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->tanButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->sqrtButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->lnButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->logButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->clearButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
    connect(m_ui->randomButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));

    populateMathFuncs();
    setModal(true);
}

AdvancedFunctionDialog::~AdvancedFunctionDialog()
{
    delete m_ui;
}

/*!
 * \brief 加载列的名单.
 * \param list 名单
 */
void AdvancedFunctionDialog::setColumnList(const QStringList &list)
{
    m_ui->columnList->clear();

    foreach (const QString& col, list)
        m_ui->columnList->addItem(col);
}

/*!
 * \brief 设置用户函数.
 * \param text 函数
 */
void AdvancedFunctionDialog::setText(const QString &text)
{
    m_ui->expressionEdit->setPlainText(text);
    setCursorToParenthesis();
}

/*!
 * \reimp
 */
void AdvancedFunctionDialog::showEvent(QShowEvent *)
{
    m_ui->expressionEdit->setFocus();
}

void AdvancedFunctionDialog::onAccepted()
{
    const QString exp = m_ui->expressionEdit->toPlainText();
    if (!exp.isEmpty()) {
        emit setExpression(exp);
        accept();
    }
}

void AdvancedFunctionDialog::onBoxButtonClicked(QAbstractButton* button)
{
    if (button == m_ui->buttonBox->button(QDialogButtonBox::Apply)) {
        const QString exp = m_ui->expressionEdit->toPlainText();
        if (!exp.isEmpty())
            emit setExpression(exp);
    }
}

void AdvancedFunctionDialog::onFuncTypeComboIndexChanged(int index)
{
    if (index == 0)
        populateMathFuncs();
    else
        populateTriangularFuncs();
}

void AdvancedFunctionDialog::onButtonPressed()
{
    QPushButton* which = qobject_cast<QPushButton*>(sender());

    if (which == m_ui->sinButton)
        appendText("Math.sin()");
    else if (which == m_ui->cosButton)
        appendText("Math.cos()");
    else if (which == m_ui->piButton)
        appendText("Math.PI");
    else if (which == m_ui->eButton)
        appendText("Math.E");
    else if (which == m_ui->tanButton)
        appendText("Math.tan()");
    else if (which == m_ui->sqrtButton)
        appendText("Math.sqrt()");
    else if (which == m_ui->lnButton)
        appendText("Math.ln()");
    else if (which == m_ui->logButton)
        appendText("Math.log()");
    else if (which == m_ui->randomButton)
        appendText("Math.random()");
    else if (which == m_ui->clearButton)
        m_ui->expressionEdit->clear();

    setCursorToParenthesis();
}

void AdvancedFunctionDialog::onFuncListDoubleClicked(QListWidgetItem* item)
{
    QScopedPointer<QTextCursor> cursor(new QTextCursor(m_ui->expressionEdit->textCursor()));
    cursor->insertText("Math." + item->text().trimmed());
    setCursorToParenthesis();
}

void AdvancedFunctionDialog::onColumnListDoubleClicked(QListWidgetItem* item)
{
    QScopedPointer<QTextCursor> cursor(new QTextCursor(m_ui->expressionEdit->textCursor()));
    cursor->insertText(item->text().trimmed());
    setCursorToParenthesis();
}

/*!
 * \brief 填充数学函数列表.
 */
void AdvancedFunctionDialog::populateMathFuncs()
{
    m_ui->funcList->clear();

    QStringList funcList;
    funcList << "abs(x)"
#ifdef RESERVE
             << "distance(d1,d2,d3,d4)"
             << "distance3D(d1,d2,d3,d4,d5,d6)"
#endif
             << "exp(x)"
             << "ln(x)"
             << "log(x)"
             << "round(x,n)"
             << "sqrt(x)";

    m_ui->funcList->addItems(funcList);
}

/*!
 * \brief 填充三角函数列表.
 */
void AdvancedFunctionDialog::populateTriangularFuncs()
{
    m_ui->funcList->clear();
    QStringList funcList;
    funcList << "acos(x)"
             << "acosh(d)"
             << "acot(d)"
             << "acoth(d)"
             << "acsc(d)"
             << "acsch(d)"
             << "asec(d)"
             << "asech(d)"
             << "asin(x)"
             << "asinh(d)"
             << "atan(x)"
             << "atan2(x,y)"
             << "atanh(d)"
             << "cos(x)"
             << "cosh(x)"
             << "cot(d)"
             << "coth(d)"
             << "csc(d)"
             << "csch(d)"
             << "secant(d)"
             << "sech(d)"
             << "sin(x)"
             << "sinh(x)"
             << "tan(x)"
             << "tanh(x)";
    m_ui->funcList->addItems(funcList);
}

/*!
 * \brief 追加函数文本.
 * \param text 文本
 */
void AdvancedFunctionDialog::appendText(const QString &text)
{
    QScopedPointer<QTextCursor> cursor(new QTextCursor(m_ui->expressionEdit->textCursor()));
    cursor->insertText(text);
}

/*!
 * \brief 移动光标到括号中.
 */
void AdvancedFunctionDialog::setCursorToParenthesis()
{
    m_ui->expressionEdit->setFocus();
    QTextCursor cursor = m_ui->expressionEdit->textCursor();
    int parenthesisIndex = m_ui->expressionEdit->toPlainText().indexOf('(');
    cursor.setPosition(parenthesisIndex + 1);
    m_ui->expressionEdit->setTextCursor(cursor);
}

}
