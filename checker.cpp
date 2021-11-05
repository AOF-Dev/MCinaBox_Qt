#include "checker.h"
#include <QEventLoop>

checker::checker(QObject *parent){}

void checker::run(){
    QEventLoop eventloop;
    check_game();
    if (link_list.count() > 0){
        new_game_downloader new_game_downloader[pthread];
        for (done = 0; done < pthread; done++){
            new_game_downloader[done].index = done;
            new_game_downloader[done].done = &done;
            new_game_downloader[done].link_list = &link_list; qDebug()<<link_list[done];
            new_game_downloader[done].path_list = &path_list;
            new_game_downloader[done].sha1_list = &sha1_list;
        }
        for (int i = 0; i < pthread; i++){
            new_game_downloader[i].start();
        }
        connect(new_game_downloader,SIGNAL(finished()),&eventloop,SLOT(quit()));
        connect(this,SIGNAL(pthread_completed()),&eventloop,SLOT(quit()));
        eventloop.exec();
    }
}

void checker::check_done(){
    if (done == link_list.count()) emit pthread_completed();
}

void checker::check_game(){
    QFile game_json_file(".minecraft/versions/"+game_name+"/"+game_name+".json");
    if (game_json_file.exists()){
        game_json_file.open(QIODevice::ReadOnly);
        QJsonParseError file_error;
        QJsonDocument json = QJsonDocument::fromJson(game_json_file.readAll(),&file_error);
        game_json_file.close();
        if (file_error.error==QJsonParseError::NoError) {
            QFile game_file(".minecraft/versions/"+game_name+"/"+game_name+".jar");
            if (game_file.exists()){
                game_file.open(QIODevice::ReadOnly);
                if (json.object().value("downloads").toObject().value("client").toString() == QCryptographicHash::hash(game_file.readAll(), QCryptographicHash::Sha1).toHex()) return;
                else {
                    qDebug()<<"game file broken";
                    game_file.close();
                    game_file.remove();
                    link_list.append(json.object().value("downloads").toObject().value("client").toObject().value("url").toString().replace("https://launcher.mojang.com","https://download.mcbbs.net"));
                    sha1_list.append(json.object().value("downloads").toObject().value("client").toObject().value("sha1").toString());
                    path_list.append(".minecraft/versions/"+game_name+"/"+game_name+".jar");
                }
            }
            else {
                link_list.append(json.object().value("downloads").toObject().value("client").toObject().value("url").toString().replace("https://launcher.mojang.com","https://download.mcbbs.net"));
                sha1_list.append(json.object().value("downloads").toObject().value("client").toObject().value("sha1").toString());
                path_list.append(".minecraft/versions/"+game_name+"/"+game_name+".jar");
            }
        }
        else qDebug()<<"json file wrong";
    }
    else qDebug()<<"json file isn't exist";
}
