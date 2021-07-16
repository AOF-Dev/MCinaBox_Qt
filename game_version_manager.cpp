#include "game_version_manager.h"
#include <QDir>

game_version_manager::game_version_manager(QObject *parent)
{

}

void game_version_manager::fresh_game_version_list(){
    this->start();
}

void game_version_manager::run(){
    QDir game_version_dir("/media/redbear/新加卷/HMCL/.minecraft/versions");
    game_version_dir.setFilter(QDir::Dirs);
    game_version_list=game_version_dir.entryList();



    emit game_version_manager::game_version_list_Changed();
}
