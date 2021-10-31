#ifndef GAME_VERSION_MANAGER_H
#define GAME_VERSION_MANAGER_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

class game_version_manager : public QThread
{
    Q_OBJECT
public:
    explicit game_version_manager(QObject *parent = nullptr);
    Q_PROPERTY(QStringList game_version_list READ to_game_version_list NOTIFY game_version_list_Changed)
    QStringList game_version_list;
    QStringList to_game_version_list(){
        return game_version_list;
    }
    void fresh_game_version_list();
    void run();
    Q_INVOKABLE QString get_game_version_string(int i){
        return game_version_list[i];
    }
signals:
    void game_version_list_Changed();
};
class install_new_game : public QObject
{
    Q_OBJECT
public:
    explicit install_new_game(QObject *parent = nullptr);
    Q_PROPERTY(QStringList snapshot_version READ to_snapshot_version NOTIFY snapshot_version_Changed)
    Q_PROPERTY(QStringList release_version READ to_release_version NOTIFY release_version_Changed)
    Q_PROPERTY(QStringList old_version READ to_old_version NOTIFY old_version_Changed)
    QStringList snapshot_version,release_version,old_version;
    QStringList snapshot_version_url,release_version_url,old_version_url;
    QStringList to_snapshot_version(){return snapshot_version;}
    QStringList to_release_version(){return release_version;}
    QStringList to_old_version(){return old_version;}
    int index[2];
    Q_INVOKABLE void set_index(int a,int b){index[1]=a; index[2]=b;}// a: snapshot,release,old; b: versions
    Q_INVOKABLE void get_new_game_list();
    Q_INVOKABLE void get_new_game_json();
public slots:
    void new_game_list_http_get_Completed(QNetworkReply* reply);
signals:
    void get_Completed();
    void get_Failed();
    void old_version_Changed();
    void release_version_Changed();
    void snapshot_version_Changed();

};

#endif // GAME_VERSION_MANAGER_H
