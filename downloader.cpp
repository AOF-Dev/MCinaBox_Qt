#include "downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

new_game_downloader::new_game_downloader(QThread *parent){}

void new_game_downloader::download_error(){
    qDebug()<<"real error";
}

void new_game_downloader::run(){
    QNetworkAccessManager *http_manager = new QNetworkAccessManager();
    QNetworkRequest get_request;
    QNetworkReply *get_reply;
    QEventLoop eventloop;
    QFile file;
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
                file.open(QIODevice::WriteOnly);
                file.write(buff);
                file.close();
                *done = *done + 1;
                index = *done;
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

