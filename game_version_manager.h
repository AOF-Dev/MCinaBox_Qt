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
    Q_PROPERTY(QStringList old_version READ to_old_version NOTIFY old_version_Changed)
    Q_PROPERTY(QStringList release_version READ to_release_version NOTIFY release_version_Changed)
    Q_PROPERTY(QStringList snapshot_version READ to_snapshot_version NOTIFY snapshot_version_Changed)
    QStringList old_version,release_version,snapshot_version;
    QStringList old_version_url,release_version_url,snapshot_version_url;
    QStringList to_old_version(){return old_version;}
    QStringList to_release_version(){return release_version;}
    QStringList to_snapshot_version(){return snapshot_version;}
    Q_INVOKABLE void get_new_game_list();
public slots:
    void http_get_completed(QNetworkReply* reply);
signals:
    void get_completed();
    void get_failed();
    void old_version_Changed();
    void release_version_Changed();
    void snapshot_version_Changed();
};

#endif // GAME_VERSION_MANAGER_H
