#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../sql/DatabaseManager.h"
#include "mainwindow.h"

class LoginView : public QWidget {
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);

private slots:
    void onLoginClicked(); // 登录按钮点击事件

private:
    QLineEdit *usernameEdit; // 用户名输入框
    QLineEdit *passwordEdit; // 密码输入框
    QPushButton *loginButton; // 登录按钮
    QLabel *statusLabel; // 状态提示标签

    void setupUI(); // 初始化界面
};

#endif // LOGINVIEW_H