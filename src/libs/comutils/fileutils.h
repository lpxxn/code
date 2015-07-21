#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "comutils_global.h"
#include "id.h"

#include <QMetaType>
#include <QString>
#include <QDir>
#include <QPluginLoader>
#include <QMap>
#include <QPair>
#include <QDebug>

class QDateTime;
class QFileInfo;
class QDir;
class QCoreApplication;

namespace CommonUtils {

class UTILS_EXPORT FileName : private QString
{
public:
    FileName();
    explicit FileName(const QFileInfo &info);
    QFileInfo toFileInfo() const;
    static FileName fromString(const QString &filename);
    static FileName fromUserInput(const QString &filename);
    QString toString() const;
    QString toUserOutput() const;

    FileName parentDir() const;

    bool operator==(const FileName &other) const;
    bool operator!=(const FileName &other) const;
    bool operator<(const FileName &other) const;
    bool operator<=(const FileName &other) const;
    bool operator>(const FileName &other) const;
    bool operator>=(const FileName &other) const;

    bool isChildOf(const FileName &s) const;
    bool isChildOf(const QDir &dir) const;
    bool endsWith(const QString &s) const;

    CommonUtils::FileName relativeChildPath(const FileName &parent) const;
    CommonUtils::FileName &appendPath(const QString &s);
    CommonUtils::FileName &append(const QString &str);
    CommonUtils::FileName &append(QChar str);

    using QString::size;
    using QString::count;
    using QString::length;
    using QString::isEmpty;
    using QString::isNull;
    using QString::clear;
private:
    FileName(const QString &string);
};

class UTILS_EXPORT FileUtils {
public:
    static QDir directoryOf(const QString &path = QString());
    static QDir directoryOfPlugin(Id appId);
    static QString librarySuffix();
    static QString filenameFilter(const QString &name, const QList<QByteArray> &formats);
    static bool copyRecursively(const FileName &srcFilePath, const FileName &tgtFilePath,
                                QString *error = nullptr);
    static bool isFileNewerThan(const FileName &filePath, const QDateTime &timeStamp);
    static FileName resolveSymlinks(const FileName &path);
    static QString shortNativePath(const FileName &path);
    static bool makeWritable(const FileName &path);
    static QString guessTextCodec(const QString& fileName, const QString& locale = "");
    static QString readTextFile(const QString& fileName, const QString& locale = "");
    static QString computeFileHash(const QString& fileName);
    static QPair<qint64, QString> fileProperties(const QString& fileName);
    static void loadTranslations(QCoreApplication* app, Id appId);
#ifdef USELOG
    static void prepareLog4qtConfFile(const QCoreApplication& app);
#endif    
};

UTILS_EXPORT QString encodeForJSON(const QString& text);
UTILS_EXPORT QDateTime compileTime();

/*!
 * \brief 加载应用 \a appId 插件目录下实现了 \a Interface 接口的插件.
 * \param appId 应用标识
 * \return 所有已加载插件的接口指针与插件元数据的关联容器对象
 */
template <typename Interface>
QMap<Interface*, QVariantMap> loadPlugins(Id appId)
{
    QMap<Interface*, QVariantMap> loadedPlugins;
    QDir pluginDir = FileUtils::directoryOf(QStringLiteral("plugins"));
    if (!pluginDir.cd(appId.toString().split('.').at(1).toLower())) {
        Q_ASSERT(0);
        return loadedPlugins;
    }

    foreach (const QString& fileName, pluginDir.entryList(QDir::Files)) {
        if (!QLibrary::isLibrary(fileName))
            continue;
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName));
        if (Interface* interface = qobject_cast<Interface*>(loader.instance()))
            loadedPlugins[interface] = loader.metaData().toVariantMap();
        else
            loader.unload();
    }
    return loadedPlugins;
}

/*!
 * \brief 加载应用 \a appId 插件目录下实现了 \a Interface 接口, 文件名是 \a pluginFile 的插件.
 * \param appId 应用标识
 * \param pluginFile 插件文件名称
 * \param error 插件加载失败时的错误信息
 * \return 加载插件的接口指针与插件元数据的数据对
 */
template <typename Interface>
QPair<Interface*, QVariantMap> loadPlugin(Id appId, const QString& pluginFile, QString* error = nullptr)
{
    QDir pluginDir = CommonUtils::FileUtils::directoryOf(QStringLiteral("plugins"));
    if (!pluginDir.cd(appId.toString().split('.').at(1).toLower())) {
        Q_ASSERT(0);
        return qMakePair(nullptr, QVariantMap());
    }
    //todo 考虑不使用librarySuffix函数
    qDebug() << pluginDir.absoluteFilePath(pluginFile + FileUtils::librarySuffix());
    QPluginLoader loader(pluginDir.absoluteFilePath(pluginFile + FileUtils::librarySuffix()));
    Interface* interface = qobject_cast<Interface*>(loader.instance());
    if (error)
        *error = loader.errorString();
    if (interface) {
        return qMakePair(interface, loader.metaData().toVariantMap());
    } else {
        loader.unload();
        return qMakePair(nullptr, QVariantMap());
    }
}

} //namespace CommonUtils

UTILS_EXPORT uint qHash(const CommonUtils::FileName &a);

Q_DECLARE_METATYPE(CommonUtils::FileName)

#endif // FILEUTILS_H
