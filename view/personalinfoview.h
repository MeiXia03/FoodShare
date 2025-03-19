#ifndef PERSONALINFOVIEW_H
#define PERSONALINFOVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>

class PersonalInfoView : public QWidget {
    Q_OBJECT

public:
    explicit PersonalInfoView(int userId, QWidget *parent = nullptr); // 接收用户ID

private slots:
    void onChangeAvatarClicked(); // 更改头像按钮点击事件
    void onUpdateSignatureClicked(); // 更新个性签名按钮点击事件

private:
    int userId; // 当前用户ID
    QLabel *avatarLabel; // 显示用户头像
    QPushButton *changeAvatarButton; // 更改头像按钮
    QLineEdit *signatureEdit; // 个性签名输入框
    QPushButton *updateSignatureButton; // 更新个性签名按钮
    QLabel *statusLabel; // 状态提示标签

    void setupUI(); // 初始化界面
    void loadUserInfo(); // 加载用户信息（头像和个性签名）
};

#endif // PERSONALINFOVIEW_H