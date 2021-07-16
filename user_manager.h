#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <QObject>
#include <QStringList>
#include <QFile>

class user_manager : public QObject
{
    Q_OBJECT
public:
    explicit user_manager(QObject *parent = nullptr);
    Q_PROPERTY(QStringList user_name_list READ to_user_name_list NOTIFY user_name_list_Changed)
    QStringList user_name_list,user_password_list,user_logging_method_list;
    QStringList to_user_name_list(){
        return user_name_list;
    }
    Q_INVOKABLE void add_user(QString user_name, QString user_password, QString logging_method);
    Q_INVOKABLE void delete_user(int i);
    void fresh_file();
    void read_file();
    QFile user_config;
    QString tmp_text;
    QStringList tmp_list;

signals:
    void user_name_list_Changed();
};

#endif // USER_MANAGER_H
