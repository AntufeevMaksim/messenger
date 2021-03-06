#include "mainwindow.h"
#include "userdata.h"

void MainWindow::on_actionSet_username_triggered(){
    set_username.reset(new SetUsername(0, &user_name, &server));
    set_username->show();
}



void MainWindow::onListMailItemClicked(QListWidgetItem *item)
{
    QString friend_name = item->text();
    user_chat.reset(new Chat(0, user_name, friend_name, &server));
    user_chat->show();
}

void MainWindow::onListMailItemDoubleClicked(QListWidgetItem *item){
    UserData::DeleteFriend(*(ui->friends), *item);
}

void MainWindow::CheckFriendsStatus(){
    QString all_friends;
    for (int i = 0; i < ui->friends->count(); ++i){
        all_friends += ui->friends->item(i)->text() + "\n";
    }
    QString friens_status = server.GetUsersStatus(all_friends);
    if(friens_status.isEmpty()){
        return;
    }

    for (int i = 0; i < ui->friends->count(); ++i) {
        if (friens_status[i] == "1"){
            ui->friends->item(i)->setBackground(Qt::green);
        }
        else{
           ui->friends->item(i)->setBackground(Qt::red);
        }
    }

}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    user_name = UserData::LoadUsername();
    ui->setupUi(this);
    UserData::LoadListFriends(*(ui->friends));
    connect(ui->friends, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onListMailItemDoubleClicked(QListWidgetItem*)));

    connect(ui->friends, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(onListMailItemClicked(QListWidgetItem*)));

    server = ServerInterface(user_name);
    connect(&check_friends_status, SIGNAL(timeout()), this, SLOT(CheckFriendsStatus()));
    check_friends_status.start(1000);

}




void MainWindow::on_actionAdd_user_triggered()
{
    add_new_friend_window.reset(new AddNewFriend(0, ui->friends, &server));
    add_new_friend_window->show();
}


void MainWindow::closeEvent(QCloseEvent*){
    server.BreakConnection();
}












