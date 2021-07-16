#ifndef START_GAME_H
#define START_GAME_H

#include <QObject>

class start_game : public QObject
{
    Q_OBJECT
public:
    explicit start_game(QObject *parent = nullptr);

signals:

};

#endif // START_GAME_H
