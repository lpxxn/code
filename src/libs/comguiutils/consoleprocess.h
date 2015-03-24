#ifndef SIMPROCESS_H
#define SIMPROCESS_H

#include "comguiutils_global.h"

#include <QObject>
#include <QProcess>

class COMGUIUTILS_EXPORT ConsoleProcess : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleProcess(QObject *parent = 0);
    virtual ~ConsoleProcess();

    enum DatumType { Stdin, Stdout, Stderr };
    typedef QPair<DatumType,QByteArray> DatumItem;
    typedef QList<DatumItem> DatumItemList;

public:
    void start();
    void terminate();

    int pid() const;
    bool isRunning() const;

    void write(const QByteArray& data);

    void setProgram(const QString& program);
    QString program() const;
    QString name() const;
    QIcon icon() const;

    void setArguments(const QStringList& args);
    QStringList arguments() const;

    void setWorkingDirectory(const QString& dir);
    QString workingDirectory() const;

    void setStandardInputFile(const QString & fileName);
    QString standardInputFile() const;

    void setStandardOutputFile(const QString & fileName);
    QString standardOutputFile() const;

    void setProcessEnvironment(const QProcessEnvironment& env);
    QProcessEnvironment processEnvironment() const;

    QByteArray inputData() const;
    QByteArray outputData() const;
    QByteArray errorData() const;
    DatumItemList allData() const;

    void clear();
    QString information() const;

signals:
    void error(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError(const QByteArray& data);
    void readyReadStandardOutput(const QByteArray& data);
    void started();
    void stateChanged(QProcess::ProcessState newState);

private:
    class ConsoleProcessPrivate* d_ptr;
    Q_DISABLE_COPY(ConsoleProcess)
};

#endif // SIMPROCESS_H
