#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>
#include <QThread>
#include <downloader.h>

class checker : public QThread
{
    Q_OBJECT
public:
    explicit checker(QObject *parent = nullptr);


signals:

};

#endif // CHECKER_H
