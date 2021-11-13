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
    Q_INVOKABLE int to_total();
    int downloading;
    Q_INVOKABLE int to_downloaded();
    Q_PROPERTY(int pthread READ to_pthread WRITE set_pthread NOTIFY pthread_Changed)
    int pthread = 30;
    int to_pthread();
    int finished_pthread;
    void set_pthread(int i);
public slots:
    void pthread_Completed_one();
    void pthread_finished();
signals:
    void fresh_downloaded();
    void download_Completed();

    void pthread_Changed();
};

#endif // CHECKER_H
