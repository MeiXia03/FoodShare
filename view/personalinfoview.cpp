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
    avatarLabel->setFixedSize(100, 100); // 设置头像大小
    avatarLabel->setStyleSheet("border: 2px solid #ccc; border-radius: 50px; background-color: #f0f0f0;");
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 更改头像按钮
    changeAvatarButton = new QPushButton("更改头像", this);
    changeAvatarButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
    connect(changeAvatarButton, &QPushButton::clicked, this, &PersonalInfoView::onChangeAvatarClicked);

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

    // 布局
    layout->addWidget(avatarLabel, 0, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(changeAvatarButton, 0, Qt::AlignRight);
    layout->addWidget(signatureEdit);
    layout->addWidget(updateSignatureButton);
    layout->addWidget(statusLabel);

    setLayout(layout);
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