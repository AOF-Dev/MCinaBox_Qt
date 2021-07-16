
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
#include "permissions.h"
#include <QMessageBox>
#include <QApplication>

Permissions::Permissions(QObject *parent) : QObject(parent)
{
}

// Method to request permissions
void Permissions::requestExternalStoragePermission()
{
    #if defined(Q_OS_ANDROID)

        QtAndroid::PermissionResult request = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
        if (request == QtAndroid::PermissionResult::Denied)
        {
            QtAndroid::requestPermissionsSync(QStringList() <<  "android.permission.READ_EXTERNAL_STORAGE");
            request = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");

            if (request == QtAndroid::PermissionResult::Denied)
            {
                this->permissionResult = false;
                if (QtAndroid::shouldShowRequestPermissionRationale("android.permission.READ_EXTERNAL_STORAGE"))
                {
                    ShowPermissionRationale = QAndroidJniObject("org/bytran/bytran/ShowPermissionRationale",
                                                                "(Landroid/app/Activity;)V",
                                                                 QtAndroid::androidActivity().object<jobject>()
                                                               );

                    // Checking for errors in the JNI
                    QAndroidJniEnvironment env;
                    if (env->ExceptionCheck()) {
                        // Handle exception here.
                        env->ExceptionClear();
                    }
                }
            }
            else { this->permissionResult = true; }
        }
        else { this->permissionResult = true; }

    #else

        this->permissionResult = false;

    #endif
}

// Method to get the permission granted state
bool Permissions::getPermissionResult() { return permissionResult; }
