#ifndef TYPEDECLARE
#define TYPEDECLARE

#include <QList>

#include <QXlsx/xlsxformat.h>
#include <QXlsx/xlsxcell.h>
#include <QXlsx/xlsxworksheet.h>
#include <docx/document.h>
#include <docx/shape.h>
#include <docx/text.h>
#include <docx/table.h>
#include <docx/headerandfooter.h>
#include <docx/section.h>
#include <docx/enums/enumtext.h>

Q_DECLARE_METATYPE(QXlsx::Format)
Q_DECLARE_METATYPE(QXlsx::Format*)
Q_DECLARE_METATYPE(QXlsx::Cell*)
Q_DECLARE_METATYPE(QXlsx::Worksheet*)
Q_DECLARE_METATYPE(Docx::Paragraph*)
Q_DECLARE_METATYPE(Docx::Table*)
Q_DECLARE_METATYPE(Docx::Run*)
Q_DECLARE_METATYPE(Docx::Cell*)
Q_DECLARE_METATYPE(QList<Docx::Cell*>)
Q_DECLARE_METATYPE(QList<Docx::Table*>)
Q_DECLARE_METATYPE(QList<Docx::Paragraph*>)
Q_DECLARE_METATYPE(Docx::InlineShape*)
Q_DECLARE_METATYPE(Docx::Footer*)
Q_DECLARE_METATYPE(Docx::Header*)
Q_DECLARE_METATYPE(Docx::Section*)
Q_DECLARE_METATYPE(Docx::WD_PARAGRAPH_ALIGNMENT)

#endif // TYPEDECLARE


