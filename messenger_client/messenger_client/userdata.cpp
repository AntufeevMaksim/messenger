#include "userdata.h"

#include "serverinterface.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/filewritestream.h>
#include "rapidjson/filereadstream.h"
#include <rapidjson/prettywriter.h>

#include <cstdio>
#include <cstring>
#include <fstream>

int UserData::LoadUserId(){
    using namespace rapidjson;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    Document doc {};
    doc.ParseStream( isw );

    StringBuffer buffer {};
    Writer<StringBuffer> writer { buffer };
    doc.Accept( writer );


    Value& Id = doc["id"];
    int userId = Id.GetInt();
    ifs.close();
    return userId;
}


void UserData::SaveUserId(int user_id){
    using namespace rapidjson;

    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();


    Value id;
    id.SetInt(user_id);
    doc["id"] = id;

    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);
}


QString UserData::LoadUsername(){

    using namespace rapidjson;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    Document doc {};
    doc.ParseStream( isw );

    StringBuffer buffer {};
    Writer<StringBuffer> writer { buffer };
    doc.Accept( writer );


    Value& name = doc["username"];
    QString username = name.GetString();
    ifs.close();
    return username;
}



void UserData::LoadListFriends(QListWidget& users, ServerInterface *server){
    using namespace rapidjson;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    Document doc {};
    doc.ParseStream( isw );

    StringBuffer buffer {};
    Writer<StringBuffer> writer { buffer };
    doc.Accept( writer );

    Value& friends = doc["my_friends"];

    int id;
    for (SizeType i = 0; i < friends.Size(); i++){
        QListWidgetItem *user = new QListWidgetItem();
        id = friends[i]["id"].GetInt();
        user->setData(Qt::UserRole, id);
        user->setText(server->GetUserName(id));
        users.addItem(user);
        users.item(0)->data(Qt::UserRole).value<int>();
    }
    ifs.close();
}

void UserData::DeleteFriend(QListWidget &users, QListWidgetItem &user){
    using namespace rapidjson;

    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();

    Value& friends = doc["my_friends"];

    for (SizeType i = 0; i < friends.Size(); i++){
        if(!std::strcmp(friends[i][0].GetString(), user.text().toStdString().c_str())){
            friends.Erase(friends.Begin() + i);
        }
    }

    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);

    delete users.takeItem(users.row(&user));
}


void UserData::SetUsername(QString &username){
    using namespace rapidjson;

    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();

    doc["username"].SetString(username.toStdString().c_str(), username.size());

    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);

}


void UserData::AddNewFriend(QString &str_name){
    using namespace rapidjson;

    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();

    Value friend_;
    friend_.SetArray();
    Value name;
    name.SetString(str_name.toStdString().c_str(), str_name.size());
    friend_.PushBack(name, doc.GetAllocator());

    doc["my_friends"].PushBack(friend_, doc.GetAllocator());

    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);
}


void UserData::AddNewMessage(int id, QString string_message, UserType& user_type){
    using namespace rapidjson;
    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();

    std::string ready_message(string_message.toStdString().c_str());

    if(user_type == this_user)
        ready_message = "u" + ready_message;
    else{
        ready_message = "f" + ready_message;
    }

    Value& users = doc["my_friends"];
    Value message;
    message.SetString(ready_message.c_str(), ready_message.size());

    for (SizeType i = 0; i < users.Size(); i++){
      if(users[i][0].GetInt() == id){
        users[i].PushBack(message, doc.GetAllocator());
        break;
      }
    }

    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);
}

void UserData::LoadChatHistory(QListWidget *chat, int id){

    using namespace rapidjson;
    Document doc;
    std::ifstream ifs { R"(userdata.json)" };

    IStreamWrapper isw { ifs };

    doc.ParseStream( isw );
    ifs.close();

    Value& users = doc["my_friends"];

    for (SizeType i = 0; i < users.Size(); i++){
        if(users[i][0].GetInt() == id){
            for (SizeType i2 = 1; i2 < users[i].Size(); i2++){
                QListWidgetItem *message = new QListWidgetItem;
                QString text = users[i][i2].GetString();
                text.remove(0,1);
                message->setText(text.toStdString().c_str());
                if(users[i][i2].GetString()[0] == 'u'){
                    message->setBackground(Qt::green);
                }
                chat->addItem(message);
            }
        }
    }


    FILE* fp = fopen("userdata.json", "w");

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));

    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);
}