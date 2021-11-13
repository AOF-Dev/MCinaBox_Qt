#include "checker.h"
#include "game_version_manager.h"
#include <QEventLoop>
#include <QThread>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>

checker::checker(QObject *parent){}

int checker::to_pthread(){return pthread;}
void checker::set_pthread(int i){pthread=i;}
int checker::to_total(){return link_list.count();}
int checker::to_downloaded(){return downloading-pthread;}

void checker::run(){
    QEventLoop eventloop;
    check_game();
    check_library();
    check_asset();
    if (link_list.count() > 0){
        finished_pthread = 0;
        new_game_downloader new_game_downloader[pthread];
        for (downloading = 0; downloading < pthread; downloading++){
            new_game_downloader[downloading].index = downloading;
            new_game_downloader[downloading].downloading = &downloading;
            new_game_downloader[downloading].link_list = &link_list; qDebug()<<link_list[downloading];
            new_game_downloader[downloading].path_list = &path_list;
            new_game_downloader[downloading].sha1_list = &sha1_list;
        }
        downloading-=1;
        emit fresh_downloaded();
        for (int i=0; i < pthread; i++){
            connect(&new_game_downloader[i],SIGNAL(finished()),this,SLOT(pthread_finished()));
        }
        for (int i=0; i < pthread; i++){
            connect(&new_game_downloader[i],SIGNAL(completed_one()),this,SLOT(pthread_Completed_one()));
        }
        connect(this,SIGNAL(download_Completed()),&eventloop,SLOT(quit()));
        for (int i = 0; i < pthread; i++){
            new_game_downloader[i].start();
        }
        eventloop.exec();
        link_list.clear();
        path_list.clear();
        sha1_list.clear();
    }
}

void checker::pthread_Completed_one(){
    qDebug()<<downloading<<"/"<<link_list.count();
    emit fresh_downloaded();
}

void checker::pthread_finished(){
    finished_pthread++;
    if (finished_pthread == pthread) emit download_Completed();
}

void checker::check_game(){
    QDir game_dir(".minecraft/versions");
    if (!game_dir.exists()) game_dir.path();
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

void checker::check_library(){
    QDir library_dir;
    QFile library_file;
    if (!library_dir.exists(".minecraft/libraries")) library_dir.mkpath(".minecraft/libraries");
    QFile game_json_file(".minecraft/versions/"+game_name+"/"+game_name+".json");
    if (game_json_file.exists()){
        game_json_file.open(QIODevice::ReadOnly);
        QJsonParseError file_error;
        QJsonDocument json = QJsonDocument::fromJson(game_json_file.readAll(),&file_error);
        game_json_file.close();
        if (file_error.error==QJsonParseError::NoError) {
            for (int i = 0; i < json.object().value("libraries").toArray().count(); i++){
                if (library_file.exists(".minecraft/libraries/"+json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("path").toString())){
                    library_file.open(QIODevice::ReadOnly);
                    if (json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("sha1").toString() == QCryptographicHash::hash(library_file.readAll(), QCryptographicHash::Sha1).toHex()) continue;
                    else {
                        qDebug()<<"library file broken";
                        library_file.close();
                        library_file.remove();
                        link_list.append(json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("url").toString().replace("https://libraries.minecraft.net","https://download.mcbbs.net/maven"));
                        sha1_list.append(json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("sha1").toString());
                        path_list.append(".minecraft/libraries/"+json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("path").toString());
                    }
                }
                else {
                    link_list.append(json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("url").toString().replace("https://libraries.minecraft.net","https://download.mcbbs.net/maven"));
                    sha1_list.append(json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("sha1").toString());
                    path_list.append(".minecraft/libraries/"+json.object().value("libraries").toArray().at(i).toObject().value("downloads").toObject().value("artifact").toObject().value("path").toString());
                }
            }
        }
        else qDebug()<<"json file wrong";
    }
    else qDebug()<<"json file isn't exist";
}

void checker::check_asset(){
    QDir asset_dir;
    QFile asset_file;
    QFile json_file(".minecraft/versions/"+game_name+"/"+game_name+".json");
    if (!asset_dir.exists(".minecraft/assets/indexes")) asset_dir.mkpath(".minecraft/assets/indexes");
    if (json_file.exists()) {
        json_file.open(QIODevice::ReadOnly);
        QJsonParseError file_error;
        QJsonDocument json = QJsonDocument::fromJson(json_file.readAll(),&file_error);
        json_file.close();
        if (file_error.error==QJsonParseError::NoError){
            if (!json_file.exists(".minecraft/assets/indexes/"+json.object().value("assetIndex").toObject().value("id").toString()+".json")){
                QNetworkAccessManager *http_manager = new QNetworkAccessManager();
                QNetworkRequest get_request;
                QNetworkReply *get_reply;
                QEventLoop eventloop;
                QObject::connect(http_manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
                get_request.setUrl(json.object().value("assetIndex").toObject().value("url").toString());
                get_request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
                get_reply = http_manager->get(get_request);
                eventloop.exec();
                if (get_reply->error() == QNetworkReply::NoError) {
                    json_file.setFileName(".minecraft/assets/indexes/"+json.object().value("assetIndex").toObject().value("id").toString()+".json");
                    json_file.open(QIODevice::WriteOnly);
                    json_file.write(get_reply->readAll());
                    json_file.close();
                }
                else qDebug()<<"get asset json failed"<<get_reply->errorString();
            }
            json_file.setFileName(".minecraft/assets/indexes/"+json.object().value("assetIndex").toObject().value("id").toString()+".json");
            json_file.open(QIODevice::ReadOnly);
            QString json_string = json_file.readAll();
            json_file.seek(0);
            json = QJsonDocument::fromJson(json_file.readAll(),&file_error);
            if (file_error.error==QJsonParseError::NoError){
                for (int i = 0; i < json_string.count(); i++){ //assets' index is too large for Qt's JSON parser to parse.Though it parses no error,it will make the program crashes.So make it by self
                    if (json_string.mid(i,4) == "hash"){
                        link_list.append(QString("http://resources.download.minecraft.net/"+json_string.mid(i+8,2)+"/"+json_string.mid(i+8,40)).replace("http://resources.download.minecraft.net","https://download.mcbbs.net/assets"));
                        sha1_list.append(json_string.mid(i+8,40));
                        path_list.append(".minecraft/assets/objects/"+json_string.mid(i+8,2)+"/"+json_string.mid(i+8,40));
                    }
                }
            }
            else qDebug()<<"asset json file error";
        }
        else qDebug()<<"game json file error";
    }
    else{
        qDebug()<<"json file isn't exist";
    }
}
