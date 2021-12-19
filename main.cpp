#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QDir>
#include <QCoreApplication>
#include <QQuickWindow>

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
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::VulkanRhi);//MUCH FASTER!!!
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //check&create game dir
    QDir game_dir;
    if(!game_dir.exists(".minecraft")) game_dir.mkpath(".minecraft");
    //load user list
    user_manager user_manager;
    user_manager.read_file();
    //load checker
    checker checker;
    //load game version list
    game_version_manager game_version_manager;
    //game_version_manager.start();
    //get new game list
    install_new_game install_new_game;
    install_new_game.checker_p = &checker;
    install_new_game.game_version_manager_p = &game_version_manager;
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
    engine.rootContext()->setContextProperty("checker",&checker);
    engine.rootContext()->setContextProperty("user_manager",&user_manager);
    engine.rootContext()->setContextProperty("install_new_game",&install_new_game);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
