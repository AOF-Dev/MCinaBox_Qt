#include "downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QDir>

new_game_downloader::new_game_downloader(QThread *parent){}



void new_game_downloader::run(){
    QNetworkAccessManager *http_manager = new QNetworkAccessManager();
    QNetworkRequest get_request;
    QNetworkReply *get_reply;
    QEventLoop eventloop;
    QFile file;
    QFileInfo file_info;
    QDir file_dir;

    connect(http_manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));

    while (index < link_list->count()) {
        get_request.setUrl(link_list->at(index)); qDebug()<<link_list->at(index);
        qDebug()<<"getting";
        get_reply = http_manager->get(get_request);
        eventloop.exec();
        qDebug()<<"gotten";
        if (get_reply->error() == QNetworkReply::NoError) {
            QByteArray buff = get_reply->readAll();
            if (sha1_list->at(index) == QCryptographicHash::hash(buff, QCryptographicHash::Sha1).toHex()){
                file.setFileName(path_list->at(index));
                file_info.setFile(file);
                file_dir.mkpath(file_info.path());
                file.open(QIODevice::WriteOnly);
                file.write(buff);
                file.close();
                *downloading+=1;
                index = *downloading; qDebug()<<index<<"/"<<link_list->count();
            }
            else{
                qDebug()<<"hash error";
            }
        }
        else {
            qDebug()<<"network error"<<get_reply->errorString();
        }
    }
}

