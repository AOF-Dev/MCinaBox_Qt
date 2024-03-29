#include "game_version_manager.h"

game_version_manager::game_version_manager(QObject *parent){}
install_new_game::install_new_game(QObject *parent){}

QString game_version_manager::get_game_version_string(int i){return game_version_list[i];}
int game_version_manager::count_game_version_list(){return game_version_list.count();}

void game_version_manager::fresh_game_version_list(){
    this->start();
}

bool game_version_manager::dir_is_empty(QString path){
    QDir dir(path);
    if (dir.isEmpty()) return true;
    else return false;
}

void game_version_manager::run(){
    QDir game_version_dir(".minecraft/versions");
    game_version_dir.setFilter(QDir::Dirs);
    game_version_list=game_version_dir.entryList();
    for (int i=0;i<game_version_list.count();i++) if (game_version_list[i]=="." | game_version_list[i]==".." | dir_is_empty(game_version_dir.path()+"/"+game_version_list[i])) {game_version_list.removeAt(i);i--;}
    emit game_version_manager::game_version_list_Changed();
}

QStringList install_new_game::to_snapshot_version(){return snapshot_version;}
QStringList install_new_game::to_release_version(){return release_version;}
QStringList install_new_game::to_old_version(){return old_version;}
void install_new_game::set_index(int a,int b){index[0]=a; index[1]=b;}// a: snapshot,release,old; b: versions
void install_new_game::set_new_game_name(QString s){new_game_name=s;}

void install_new_game::get_new_game_list(){
    QNetworkAccessManager *http_manager = new QNetworkAccessManager();
    QNetworkRequest get_request(QUrl(QString("https://launchermeta.mojang.com/mc/game/version_manifest.json")/*.replace("https://launchermeta.mojang.com","https://download.mcbbs.net")*/));
    get_request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QObject::connect(http_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(new_game_list_http_get_Completed(QNetworkReply*)));
    http_manager->get(get_request);
}

void install_new_game::new_game_list_http_get_Completed(QNetworkReply* reply){
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
        emit install_new_game::get_Failed();
    }
    else {
        QJsonDocument data = QJsonDocument::fromJson(reply->readAll());
        QJsonObject json = data.object();
        QJsonArray versions = json.value("versions").toArray();
        for (int i=0;i<versions.count();i++) {
            if (versions.at(i).toObject().value("type").toString().contains("snapshot")) {
                snapshot_version.append(versions.at(i).toObject().value("id").toString());
                snapshot_version_url.append(versions.at(i).toObject().value("url").toString());
            }
            else if (versions.at(i).toObject().value("type").toString().contains("old")) {
                old_version.append(versions.at(i).toObject().value("id").toString());
                old_version_url.append(versions.at(i).toObject().value("url").toString());
            }
            else {
                release_version.append(versions.at(i).toObject().value("id").toString());
                release_version_url.append(versions.at(i).toObject().value("url").toString());
            }
        }
        emit snapshot_version_Changed();
        emit release_version_Changed();
        emit old_version_Changed();
        emit install_new_game::get_Completed();
    }
}

void install_new_game::get_new_game_json(){ //then start checker
    QNetworkAccessManager *http_manager = new QNetworkAccessManager();
    QNetworkRequest get_request;
    switch (index[0]) {
    case 1:
        get_request.setUrl(snapshot_version_url[index[1]]);
        break;
    case 2:
        get_request.setUrl(release_version_url[index[1]]);
        break;
    case 3:
        get_request.setUrl(old_version_url[index[1]]);
        break;
    }
    get_request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply *get_reply;
    QObject::connect(http_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(new_game_json_http_get_Completed(QNetworkReply*)));
    get_reply=http_manager->get(get_request);
}

void install_new_game::new_game_json_http_get_Completed(QNetworkReply* reply){
    QDir game_dir;
    if(!game_dir.exists(".minecraft/versions/"+new_game_name)) game_dir.mkpath(".minecraft/versions/"+new_game_name);
    QFile game_json(".minecraft/versions/"+new_game_name+"/"+new_game_name+".json");
    game_json.open(QIODevice::WriteOnly);
    game_json.write(reply->readAll());
    game_json.close();
    checker_p->game_name = new_game_name;
    checker_p->run();
    game_version_manager_p->fresh_game_version_list();
}
