#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QThread>

class new_game_downloader : public QThread
{
    Q_OBJECT
        public:
                 explicit new_game_downloader(QObject *parent = nullptr);

signals:

};

#endif // DOWNLOADER_H
