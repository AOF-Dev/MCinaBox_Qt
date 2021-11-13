#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QThread>

#include "checker.h"

class new_game_downloader : public QThread
{
    Q_OBJECT
public:
    explicit new_game_downloader(QThread *parent = nullptr);
    QStringList *link_list,*path_list,*sha1_list;
    int index;
    int *downloading;
    bool sha1_passed = false;
    void run();
public slots:

signals:
    void completed_one();
};

#endif // DOWNLOADER_H
