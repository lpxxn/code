#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include "clearlabel.h"
#include "comguiutils_global.h"

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>

class COMGUIUTILS_EXPORT SearchEdit : public QWidget
{
    Q_OBJECT
public:
    explicit SearchEdit(QWidget *parent = nullptr);

public:
    void setText(const QString& str);
    void clear();
    QString text() const;

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QSize sizeHint() const
    {
        return QSize(300,40);
    }

private:
    void drawBackground(QPainter* paitner);

private:
    void calcGeo();
    void initVariables();
    void initWidgets();

private:
    QLabel* m_searchLabel;
    ClearLabel* m_clearLabel;
    QLineEdit* m_searchEdit;

    bool m_bEnter;

private Q_SLOTS:
    void handleTextChanged(const QString& str);
    void clearText();

Q_SIGNALS:
    void textChanged(const QString& str);
};

#endif // SEARCHEDIT_H
