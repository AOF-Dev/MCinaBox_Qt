#include "user_manager.h"

user_manager::user_manager(QObject *parent) : QObject(parent)
{

}

void user_manager::add_user(QString user_name, QString user_password, QString logging_method){
    user_name_list.append(user_name);
    user_password_list.append(user_password);
    user_logging_method_list.append(logging_method);
    fresh_file();
    emit user_name_list_Changed();
}
void user_manager::delete_user(int i){
    user_name_list.removeAt(i);
    user_password_list.removeAt(i);
    user_logging_method_list.removeAt(i);
    fresh_file();
    emit user_name_list_Changed();
}
void user_manager::fresh_file(){
    user_config.seek(0);
    user_config.resize(0);
    for (int i=0;i<user_name_list.count();i++){
        user_config.write(user_name_list[i].toUtf8()+'\n'+user_password_list[i].toUtf8()+'\n'+user_logging_method_list[i].toUtf8()+'\n');
    }
    user_config.resize(user_config.size()-1);//delete the last '\n'.if not,it will read_file wrongly
}
void user_manager::read_file(){
    user_config.setFileName("./user.conf");
    user_config.open(QIODevice::ReadWrite);
    if (user_config.readAll()!=""){
        user_config.seek(0);
        tmp_text = user_config.readAll();
        tmp_list = tmp_text.split('\n');
        for (int i=0;i<tmp_list.count();i++) {
            user_name_list.append(tmp_list[i]); i++;
            user_password_list.append(tmp_list[i]); i++;
            user_logging_method_list.append(tmp_list[i]);
        }
    }
}
