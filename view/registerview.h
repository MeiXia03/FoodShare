#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../sql/DatabaseManager.h"

class RegisterView : public QWidget {
    Q_OBJECT

public:
    explicit RegisterView(QWidget *parent = nullptr);

private slots:
    void onRegisterClicked(); // 注册按钮点击事件

private:
    QLineEdit *usernameEdit; // 用户名输入框
    QLineEdit *passwordEdit; // 密码输入框
    QPushButton *registerButton; // 注册按钮
    QLabel *statusLabel; // 状态提示标签

    void setupUI(); // 初始化界面
};

#endif // REGISTERVIEW_H