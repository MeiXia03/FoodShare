#ifndef CHATROOMVIEW_H
#define CHATROOMVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

class ChatRoomView : public QWidget {
    Q_OBJECT

public:
    explicit ChatRoomView(int userId, QWidget *parent = nullptr); // 接收 userId

private:
    int loggedInUserId; // 当前登录用户ID
    QListWidget *friendsList; // 好友列表
    QListWidget *messagesList; // 消息记录列表
    QLineEdit *messageInput; // 消息输入框
    QPushButton *sendMessageButton; // 发送消息按钮
    QListWidget *friendRequestsList; // 好友请求列表
    QPushButton *acceptRequestButton; // 接受好友请求按钮
    QPushButton *rejectRequestButton; // 拒绝好友请求按钮
    
    void setupUI(); // 初始化界面
    void loadFriends(); // 加载好友列表
    void loadMessages(int friendId); // 加载与某好友的消息记录
    void loadFriendRequests(); // 加载好友请求

private slots:
    void onAcceptRequestClicked(); // 接受好友请求
    void onRejectRequestClicked(); // 拒绝好友请求
    void onFriendSelected();       // 选择好友时加载消息
    void onSendMessageClicked();   // 发送消息

};

#endif // CHATROOMVIEW_H