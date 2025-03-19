#include "chatroomview.h"

ChatRoomView::ChatRoomView(int userId, QWidget *parent)
    : QWidget(parent), loggedInUserId(userId) {
    setupUI();
    loadFriendRequests(); // 加载好友请求
    loadFriends(); // 加载好友列表
}

void ChatRoomView::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 好友请求列表
    friendRequestsList = new QListWidget(this);

    // 接受好友请求按钮
    acceptRequestButton = new QPushButton("接受", this);
    connect(acceptRequestButton, &QPushButton::clicked, this, &ChatRoomView::onAcceptRequestClicked);

    // 拒绝好友请求按钮
    rejectRequestButton = new QPushButton("拒绝", this);
    connect(rejectRequestButton, &QPushButton::clicked, this, &ChatRoomView::onRejectRequestClicked);

    // 好友列表
    friendsList = new QListWidget(this);
    connect(friendsList, &QListWidget::itemClicked, this, &ChatRoomView::onFriendSelected);

    // 消息记录列表
    messagesList = new QListWidget(this);

    // 消息输入框
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("输入消息...");

    // 发送消息按钮
    sendMessageButton = new QPushButton("发送", this);
    connect(sendMessageButton, &QPushButton::clicked, this, &ChatRoomView::onSendMessageClicked);

    // 布局
    layout->addWidget(new QLabel("好友请求", this));
    layout->addWidget(friendRequestsList);
    layout->addWidget(acceptRequestButton);
    layout->addWidget(rejectRequestButton);
    layout->addWidget(new QLabel("好友列表", this));
    layout->addWidget(friendsList);
    layout->addWidget(new QLabel("消息记录", this));
    layout->addWidget(messagesList);
    layout->addWidget(messageInput);
    layout->addWidget(sendMessageButton);

    setLayout(layout);
}

void ChatRoomView::loadFriends() {
    friendsList->clear();

    QSqlQuery query;
    query.prepare("SELECT f.friend_user_id, u.username FROM friends f "
                  "JOIN users u ON f.friend_user_id = u.user_id "
                  "WHERE f.user_id = :user_id AND f.status = 'accepted'");
    query.bindValue(":user_id", loggedInUserId);

    if (!query.exec()) {
        qDebug() << "加载好友列表失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int friendId = query.value("friend_user_id").toInt();
        QString friendName = query.value("username").toString();

        QListWidgetItem *item = new QListWidgetItem(friendName, friendsList);
        item->setData(Qt::UserRole, friendId); // 将好友ID存储到列表项中
    }
}

void ChatRoomView::loadMessages(int friendId) {
    messagesList->clear();

    QSqlQuery query;
    query.prepare("SELECT sender_id, content, created_at FROM messages "
                  "WHERE (sender_id = :user_id AND receiver_id = :friend_id) "
                  "OR (sender_id = :friend_id AND receiver_id = :user_id) "
                  "ORDER BY created_at ASC");
    query.bindValue(":user_id", loggedInUserId);
    query.bindValue(":friend_id", friendId);

    if (!query.exec()) {
        qDebug() << "加载消息记录失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int senderId = query.value("sender_id").toInt();
        QString content = query.value("content").toString();
        QString timestamp = query.value("created_at").toString();

        QString message = QString("[%1] %2: %3")
                              .arg(timestamp)
                              .arg(senderId == loggedInUserId ? "我" : "对方")
                              .arg(content);

        messagesList->addItem(message);
    }
}

void ChatRoomView::onFriendSelected() {
    QListWidgetItem *selectedItem = friendsList->currentItem();
    if (!selectedItem) return;

    int friendId = selectedItem->data(Qt::UserRole).toInt();
    loadMessages(friendId); // 加载与该好友的消息记录
}

void ChatRoomView::onSendMessageClicked() {
    QListWidgetItem *selectedItem = friendsList->currentItem();
    if (!selectedItem) {
        qDebug() << "未选择好友，无法发送消息";
        return;
    }

    int friendId = selectedItem->data(Qt::UserRole).toInt();
    QString messageContent = messageInput->text().trimmed();

    if (messageContent.isEmpty()) {
        qDebug() << "消息内容不能为空";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO messages (sender_id, receiver_id, content, created_at) "
                  "VALUES (:sender_id, :receiver_id, :content, CURRENT_TIMESTAMP)");
    query.bindValue(":sender_id", loggedInUserId);
    query.bindValue(":receiver_id", friendId);
    query.bindValue(":content", messageContent);

    if (!query.exec()) {
        qDebug() << "发送消息失败:" << query.lastError().text();
        return;
    }

    qDebug() << "消息发送成功";
    messageInput->clear();
    loadMessages(friendId); // 重新加载消息记录
}

void ChatRoomView::loadFriendRequests() {
    friendRequestsList->clear();

    QSqlQuery query;
    query.prepare("SELECT f.friend_id, u.username FROM friends f "
                  "JOIN users u ON f.user_id = u.user_id "
                  "WHERE f.friend_user_id = :user_id AND f.status = 'pending'");
    query.bindValue(":user_id", loggedInUserId);

    if (!query.exec()) {
        qDebug() << "加载好友请求失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int friendId = query.value("friend_id").toInt();
        QString friendName = query.value("username").toString();

        QListWidgetItem *item = new QListWidgetItem(friendName, friendRequestsList);
        item->setData(Qt::UserRole, friendId); // 将好友关系ID存储到列表项中
    }
}

void ChatRoomView::onAcceptRequestClicked() {
    QListWidgetItem *selectedItem = friendRequestsList->currentItem();
    if (!selectedItem) {
        qDebug() << "未选择好友请求";
        return;
    }

    int friendId = selectedItem->data(Qt::UserRole).toInt();

    QSqlQuery query;
    query.prepare("UPDATE friends SET status = 'accepted' WHERE friend_id = :friend_id");
    query.bindValue(":friend_id", friendId);

    if (!query.exec()) {
        qDebug() << "接受好友请求失败:" << query.lastError().text();
        return;
    }

    qDebug() << "好友请求已接受";
    loadFriendRequests(); // 重新加载好友请求
    loadFriends();        // 重新加载好友列表
}

void ChatRoomView::onRejectRequestClicked() {
    QListWidgetItem *selectedItem = friendRequestsList->currentItem();
    if (!selectedItem) {
        qDebug() << "未选择好友请求";
        return;
    }

    int friendId = selectedItem->data(Qt::UserRole).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM friends WHERE friend_id = :friend_id");
    query.bindValue(":friend_id", friendId);

    if (!query.exec()) {
        qDebug() << "拒绝好友请求失败:" << query.lastError().text();
        return;
    }

    qDebug() << "好友请求已拒绝";
    loadFriendRequests(); // 重新加载好友请求
    loadFriends();        // 重新加载好友列表
}