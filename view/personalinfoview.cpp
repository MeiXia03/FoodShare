#include "loginview.h"
#include "personalinfoview.h"
#include <QMessageBox>

PersonalInfoView::PersonalInfoView(int userId, QWidget *parent)
    : QWidget(parent), userId(userId) {
    setupUI();
    loadUserInfo(); // 加载用户信息
}

void PersonalInfoView::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 用户头像
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(100, 100); // 缩小头像大小
    avatarLabel->setStyleSheet("border: 2px solid #ccc; border-radius: 40px; background-color: #f0f0f0;"); // 圆形头像
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 更改头像按钮
    changeAvatarButton = new QPushButton("更改头像", this);
    changeAvatarButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
    connect(changeAvatarButton, &QPushButton::clicked, this, &PersonalInfoView::onChangeAvatarClicked);

    // 图片按钮
    feedbackButton = new QPushButton(this);
    feedbackButton->setFixedSize(30, 30); // 设置图片按钮大小为 30px
    feedbackButton->setStyleSheet("border: none; background-color: transparent;");
    QPixmap feedbackPixmap(":/loadFeedback.jpg");
    if (!feedbackPixmap.isNull()) {
        feedbackButton->setIcon(QIcon(feedbackPixmap));
        feedbackButton->setIconSize(QSize(30, 30)); // 设置图片大小为 30px
    }
    connect(feedbackButton, &QPushButton::clicked, this, &PersonalInfoView::onFeedbackButtonClicked);

    // 创建一个水平布局，用于放置图片按钮和更改头像按钮
    QHBoxLayout *avatarButtonLayout = new QHBoxLayout();
    avatarButtonLayout->addWidget(feedbackButton); // 添加图片按钮
    avatarButtonLayout->addStretch(); // 添加弹性空间
    avatarButtonLayout->addWidget(changeAvatarButton); // 添加更改头像按钮

    // 好友用户名输入框
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("输入好友用户名...");
    usernameInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");

    // 添加好友按钮
    addFriendButton = new QPushButton("添加好友", this);
    addFriendButton->setStyleSheet("padding: 5px 10px; background-color: #FF9800; color: white; border-radius: 5px;");
    connect(addFriendButton, &QPushButton::clicked, this, &PersonalInfoView::onAddFriendClicked);

    // 个性签名输入框
    signatureEdit = new QLineEdit(this);
    signatureEdit->setPlaceholderText("请输入个性签名...");
    signatureEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");

    // 更新个性签名按钮
    updateSignatureButton = new QPushButton("更新个性签名", this);
    updateSignatureButton->setStyleSheet("padding: 5px 10px; background-color: #2196F3; color: white; border-radius: 5px;");
    connect(updateSignatureButton, &QPushButton::clicked, this, &PersonalInfoView::onUpdateSignatureClicked);

    // 状态提示标签
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red; margin-top: 10px;");

    // 退出登录按钮
    logoutButton = new QPushButton("退出登录", this);
    logoutButton->setStyleSheet("padding: 10px; background-color: #f44336; color: white; border: none; border-radius: 5px;");
    connect(logoutButton, &QPushButton::clicked, this, &PersonalInfoView::onLogoutClicked);

    // 布局
    layout->addWidget(avatarLabel, 0, Qt::AlignRight | Qt::AlignTop);
    layout->addLayout(avatarButtonLayout); // 添加图片按钮和更改头像按钮的水平布局
    layout->addWidget(signatureEdit);
    layout->addWidget(updateSignatureButton);
    layout->addWidget(usernameInput);
    layout->addWidget(addFriendButton);
    layout->addWidget(statusLabel);
    layout->addWidget(logoutButton); // 添加退出登录按钮

    setLayout(layout);
}

void PersonalInfoView::onAddFriendClicked() {
    QString friendUsername = usernameInput->text().trimmed();

    if (friendUsername.isEmpty()) {
        statusLabel->setText("用户名不能为空！");
        return;
    }

    // 查询好友用户ID
    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE username = :username");
    query.bindValue(":username", friendUsername);

    if (!query.exec() || !query.next()) {
        statusLabel->setText("用户不存在！");
        return;
    }

    int friendUserId = query.value("user_id").toInt();

    // 检查是否已经是好友
    query.prepare("SELECT * FROM friends WHERE user_id = :user_id AND friend_user_id = :friend_user_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":friend_user_id", friendUserId);

    if (query.exec() && query.next()) {
        statusLabel->setText("该用户已经是您的好友！");
        return;
    }

    // 添加好友关系
    query.prepare("INSERT INTO friends (user_id, friend_user_id, status, created_at) "
                  "VALUES (:user_id, :friend_user_id, 'pending', CURRENT_TIMESTAMP)");
    query.bindValue(":user_id", userId);
    query.bindValue(":friend_user_id", friendUserId);

    if (!query.exec()) {
        statusLabel->setText("添加好友失败！");
        return;
    }

    QMessageBox::information(this, "成功", "好友请求已发送！");
    usernameInput->clear();
}

void PersonalInfoView::loadUserInfo() {
    // 从数据库加载用户头像和个性签名
    QSqlQuery query;
    query.prepare("SELECT avatar_path, signature FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec() || !query.next()) {
        statusLabel->setText("加载用户信息失败！");
        return;
    }

    QString avatarPath = query.value("avatar_path").toString();
    QString signature = query.value("signature").toString();

    // 加载头像
    QPixmap avatarPixmap(avatarPath);
    if (!avatarPixmap.isNull()) {
        avatarLabel->setPixmap(avatarPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        avatarLabel->setText("无头像");
    }

    // 加载个性签名
    signatureEdit->setText(signature);
}

void PersonalInfoView::onChangeAvatarClicked() {
    // 打开文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(this, "选择头像", "", "Images (*.png *.jpg *.jpeg)");
    if (filePath.isEmpty()) {
        return;
    }

    // 更新数据库中的头像路径
    QSqlQuery query;
    query.prepare("UPDATE users SET avatar_path = :avatar_path WHERE user_id = :user_id");
    query.bindValue(":avatar_path", filePath);
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "更新头像失败！");
        return;
    }

    // 更新界面上的头像
    QPixmap avatarPixmap(filePath);
    if (!avatarPixmap.isNull()) {
        avatarLabel->setPixmap(avatarPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QMessageBox::information(this, "成功", "头像更新成功！");
}

void PersonalInfoView::onUpdateSignatureClicked() {
    QString newSignature = signatureEdit->text();

    if (newSignature.isEmpty()) {
        statusLabel->setText("个性签名不能为空！");
        return;
    }

    // 更新数据库中的个性签名
    QSqlQuery query;
    query.prepare("UPDATE users SET signature = :signature WHERE user_id = :user_id");
    query.bindValue(":signature", newSignature);
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "更新个性签名失败！");
        return;
    }

    QMessageBox::information(this, "成功", "个性签名更新成功！");
}

void PersonalInfoView::onLogoutClicked() {
    // 显示登录界面
    LoginView *loginView = new LoginView();
    loginView->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动释放内存
    loginView->show();

    // 关闭主窗口
    QWidget *mainWindow = this->window(); // 获取主窗口
    if (mainWindow) {
        mainWindow->close();
    }
}

void PersonalInfoView::onFeedbackButtonClicked() {
    // 打开反馈界面
    FeedbackView *feedbackView = new FeedbackView(userId, this);
    feedbackView->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动释放内存
    feedbackView->show();
}