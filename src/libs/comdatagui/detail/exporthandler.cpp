#include "exporthandler.h"

namespace ComDataGui
{

ExportThread::ExportThread(const QString &file, ComData::Table *table, QObject* parent) : QObject(parent),
    m_fileName(file), m_table(table)
{

}

void ExportThread::setFunctor(FunctorCall call)
{
    m_functor = call;
}

void ExportThread::run()
{
    if (m_functor)
        m_functor(m_fileName, m_table);
    emit finished();
}

}
