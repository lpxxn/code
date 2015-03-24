#include <comutils/fileutils.h>
#include <comutils/licensemanager.h>

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QScriptEngine>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // 添加插件路径
    QCoreApplication::addLibraryPath(CommonUtils::FileUtils::directoryOf("plugins").absolutePath());

    CommonUtils::LicenseManager* licMng = CommonUtils::LicenseManager::instance();
    licMng->setCheckBadDate(true);
    licMng->setDefaultLicensePath("d:/license/simcube/simcube.lic");

    QScriptEngine engine;
    qDebug() << engine.availableExtensions();
    engine.importExtension("office");

    QFile file(":/testoffice.js");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    engine.evaluate(file.readAll());

    return app.exec();
}
