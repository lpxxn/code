#ifndef XLSXDOCUMENT_H
#define XLSXDOCUMENT_H

#include <QXlsx/xlsxformat.h>

#include <QVariant>
#include <QScriptValue>
#include <QScriptable>

namespace QXlsx {
    class Document;
}

class QIODevice;

class XlsxDocument : public QObject, public QScriptable
{
    Q_OBJECT
    Q_ENUMS(SheetType)
public:
    enum SheetType {
        WorkSheet,
        ChartSheet,
        DialogSheet,
        MacroSheet
    };

    XlsxDocument(QObject *parent = nullptr);
    explicit XlsxDocument(const QString& name, QObject *parent = nullptr);
    explicit XlsxDocument(QIODevice* device, QObject *parent = nullptr);
    ~XlsxDocument();

    //bool addConditionalFormatting(const ConditionalFormatting &cf);
    //bool addDataValidation(const DataValidation &validation);
    Q_INVOKABLE bool addSheet(const QString &name = QString(), SheetType type = WorkSheet);
    Q_INVOKABLE QScriptValue cellAt() const;

    Q_INVOKABLE bool copySheet(const QString& srcName, const QString& distName);

    Q_INVOKABLE bool deleteSheet(const QString & name);
    Q_INVOKABLE bool insertSheet(int index, const QString &name = QString(), SheetType type = WorkSheet);
    Q_INVOKABLE bool renameSheet(const QString &oldName, const QString &newName);
    Q_INVOKABLE bool moveSheet(const QString &srcName, int distIndex);
    Q_INVOKABLE QString documentProperty(const QString& key) const;
    Q_INVOKABLE QStringList	documentPropertyNames() const;

    Q_INVOKABLE QVariant read() const;

    Q_INVOKABLE bool save() const;
    Q_INVOKABLE bool saveAs(const QString & name) const;
    Q_INVOKABLE bool selectSheet(const QString& name);
    Q_INVOKABLE void setDocumentProperty(const QString& key, const QString& property);
    Q_INVOKABLE QStringList sheetNames() const;

    Q_INVOKABLE QScriptValue worksheet(const QString& sheetName) const;
    Q_INVOKABLE bool write();

    Q_INVOKABLE QScriptValue rowCount() const;
    Q_INVOKABLE QScriptValue columnCount() const;
    Q_INVOKABLE QScriptValue currentWorksheet() const;
    Q_INVOKABLE bool mergeCells(const QString &range, const QXlsx::Format &format = QXlsx::Format());
    Q_INVOKABLE bool unmergeCells(const QString &range);
    Q_INVOKABLE bool insertImage(int row, int col, const QString &imgPath);
    Q_INVOKABLE bool insertImage(int row, int col, const QImage &img);

    Q_INVOKABLE bool setColumnWidth();

    Q_INVOKABLE bool setColumnFormat();

    Q_INVOKABLE bool setColumnHidden();

    Q_INVOKABLE double columnWidth(int column);
    Q_INVOKABLE QScriptValue columnFormat(int column);
    Q_INVOKABLE bool isColumnHidden(int column);

    Q_INVOKABLE bool setRowHeight();
    Q_INVOKABLE bool setRowFormat();
    Q_INVOKABLE bool setRowHidden();

    Q_INVOKABLE double rowHeight(int row);
    Q_INVOKABLE QScriptValue rowFormat(int row);
    Q_INVOKABLE bool isRowHidden(int row);

    Q_INVOKABLE bool groupRows(int rowFirst, int rowLast, bool collapsed = true);
    Q_INVOKABLE bool groupColumns(int colFirst, int colLast, bool collapsed = true);

    Q_INVOKABLE bool defineName(const QString &name, const QString &formula, const QString &comment=QString(), const QString &scope=QString());

private:
    QXlsx::Document* m_document;
};

#endif // XLSXDOCUMENT_H
