#ifndef GAME_VERSION_MANAGER_H
#define GAME_VERSION_MANAGER_H

#include <QObject>
#include <QThread>
#include <QStringList>

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

#endif // GAME_VERSION_MANAGER_H
