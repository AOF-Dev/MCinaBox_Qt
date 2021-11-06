#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>

#include "downloader.h"

class checker : public QThread
{
    Q_OBJECT
public:
    explicit checker(QObject *parent = nullptr);
    QString game_name;
    void run();
    void check_game();
    void check_library();
    void check_asset();
    QStringList link_list,path_list,sha1_list;
    int downloaded;
    int downloading;
    int pthread = 1;
public slots:
    void pthread_Completed();

signals:
    void download_Completed();
};

#endif // CHECKER_H
