#ifndef EXPORTHANDLER_H
#define EXPORTHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#include <comdata/column.h>
#include <comdata/table.h>

namespace ComDataGui
{

enum { Excel, Html };

template <int type>
class Generator;

template <>
class Generator<Excel>
{
public:
    static bool write(const QString& fileName, ComData::Table* table)
    {
        Q_UNUSED(fileName)
        Q_UNUSED(table)
        return true;
    }
};

template <>
class Generator<Html>
{
public:
    static bool write(const QString& fileName, ComData::Table* table)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            return false;

        QFile styleFile(":/res/tablestyle.css");
        if (!styleFile.open(QIODevice::ReadOnly))
            qDebug() << "failed to read html style file.";
        QString style = styleFile.readAll();

        QTextStream ts(&file);
        ts << "<!DOCTYPE html><html><head>";
        ts << "<style>" << style << "</style></head><body><table class=\"bordered\">";
        ts << "<thead><tr><th>" << QObject::tr("Name") << "</th>";

        for (int i = 0; i < table->size(); ++i)
            ts << "<th>" << table->at(i)->name() << "</th>";

        ts << "</tr></thead>";

        int row = 0;
        if (table->size() > 0)
            row = table->at(0)->size();

        // long name
        ts << "<tr><td>" << QObject::tr("Long Name") << "</td>";
        for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
            ts << "<td>" << table->at(colIndex)->longName() << "</td>";
        }
        ts << "</tr>";

        // unit
        ts << "<tr><td>" << QObject::tr("Unit") << "</td>";
        for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
            ts << "<td>" << table->at(colIndex)->unit() << "</td>";
        }
        ts << "</tr>";

        // comment
        ts << "<tr><td>" << QObject::tr("Comment") << "</td>";
        for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
            ts << "<td>" << table->at(colIndex)->comment() << "</td>";
        }
        ts << "</tr>";

        // data
        for (int rowIndex = 0; rowIndex < row; ++rowIndex) {
            ts << "<tr><td>" << rowIndex + 1 << "</td>";
            for (int colIndex = 0; colIndex < table->size(); ++colIndex) {
                QSharedPointer<ComData::ColumnBase> colData = table->at(colIndex);
                ts << "<td>" << colData->cell(rowIndex).toString() << "</td>";
            }
        }

        ts << "</table></body></html>";
    }
};

typedef bool (*FunctorCall)(const QString&, ComData::Table*);

/*!
 * \class ExportThread
 * \brief 导出表格数据的线程.
 */
class ExportThread : public QObject
{
Q_OBJECT
public:
    explicit ExportThread(const QString& file, ComData::Table* table, QObject* parent = nullptr);

    void setFunctor(FunctorCall call);

public slots:
    void run();

signals:
    void finished();

private:
    QString m_fileName;
    ComData::Table* m_table;
    FunctorCall m_functor = nullptr;
};

}
#endif // EXPORTHANDLER_H
