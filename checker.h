#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QCryptographicHash>

#include "downloader.h"

class checker : public QThread
{
    Q_OBJECT
public:
    explicit checker(QObject *parent = nullptr);
    QString game_name;
    void run();
    void check_game();
    QStringList link_list,path_list,sha1_list;
    int done;
    int pthread = 1;
public slots:
    void check_done();

signals:
    void pthread_completed();
};

#endif // CHECKER_H
