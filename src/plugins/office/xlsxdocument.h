#ifndef XLSXDOCUMENT_H
#define XLSXDOCUMENT_H

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

private:
    QXlsx::Document* m_document;
};

#endif // XLSXDOCUMENT_H
