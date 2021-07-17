#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include "game_version_manager.h"
#include "user_manager.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
bool checkPermission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
    return true;
}
bool checkPermission2() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.READ_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
    return true;
}
#endif

int main(int argc, char *argv[])
{

#ifdef Q_OS_ANDROID
    checkPermission();
    checkPermission2();
#endif

    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));  //Qt virtualkeyboard
    QQuickStyle::setStyle("Material");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //load user list
    user_manager user_manager;
    user_manager.read_file();
    //load game version list
    game_version_manager game_version_manager;
    game_version_manager.fresh_game_version_list();


    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MCinaBox_Qt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("game_version_manager",&game_version_manager);
    engine.rootContext()->setContextProperty("user_manager",&user_manager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}