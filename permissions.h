
// The following resources where most helpful in implementing this permission request code insert
// https://doc.qt.io/qt-5/qtandroid.html
// http://falsinsoft.blogspot.com.by/2015/03/qt-android-interface-c-and-java-through.html
// http://www.kdab.com/qt-android-episode-5/
// http://www.kdab.com/qt-android-episode-6/
// https://www.kdab.com/qt-android-episode-7/
// http://stackoverflow.com/questions/35027043/implementing-android-6-0-permissions-in-unity3d
// http://stackoverflow.com/questions/32347532/android-m-permissions-confused-on-the-usage-of-shouldshowrequestpermissionrati
// https://developer.android.com/training/permissions/requesting.html
// https://inthecheesefactory.com/blog/things-you-need-to-know-about-android-m-permission-developer-edition/en
// https://gist.github.com/patrickhammond/47cff299bc34e976e18b
// https://github.com/codepath/android_guides/wiki/Understanding-App-Permissions
// http://doc.qt.io/qt-5/qandroidjniobject.html




// Object to request permissions from the Android system
#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <QObject>
#ifdef Q_OS_ANDROID
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
    #include <QtAndroid>
#endif

class Permissions : public QObject
{
    Q_OBJECT
public:
    explicit Permissions(QObject *parent = 0);

    // Method to request permissions
    void requestExternalStoragePermission();

    // Method to get the permission granted state
    bool getPermissionResult();

public slots:

private:

    // Variable indicating if the permission to read / write has been granted
    int permissionResult;  //  true - "Granted", false - "Denied"

#if defined(Q_OS_ANDROID)

    // Object used to obtain permissions on Android Marshmallow
    QAndroidJniObject ShowPermissionRationale;

#endif

};

#endif // PERMISSIONS_H
