#include "registerview.h"
#include <QMessageBox>

RegisterView::RegisterView(QWidget *parent) : QWidget(parent) {
    setupUI();
    setWindowTitle("用户注册"); // 设置窗口标题
    resize(400, 300); // 设置窗口默认大小

    setWindowModality(Qt::ApplicationModal); // 设置模态，阻止与其他窗口交互
}

void RegisterView::setupUI() {
    // 创建标题标签
    QLabel *titleLabel = new QLabel("用户注册", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #4CAF50; margin-bottom: 20px;");

    // 创建用户名输入框
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");
    usernameEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建密码输入框
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建注册按钮
    registerButton = new QPushButton("注册", this);
    registerButton->setStyleSheet("padding: 10px; background-color: #2196F3; color: white; border: none; border-radius: 5px;");
    connect(registerButton, &QPushButton::clicked, this, &RegisterView::onRegisterClicked);

    // 创建状态提示标签
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red; margin-top: 10px;");

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(new QLabel("用户名:", this));
    layout->addWidget(usernameEdit);
    layout->addWidget(new QLabel("密码:", this));
    layout->addWidget(passwordEdit);
    layout->addWidget(registerButton);
    layout->addWidget(statusLabel);

    setLayout(layout);
}

void RegisterView::onRegisterClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("用户名或密码不能为空！");
        return;
    }

    // 插入新用户
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        statusLabel->setText("注册失败，用户名可能已存在！");
        return;
    }

    QMessageBox::information(this, "成功", "注册成功，请返回登录！");
    this->close(); // 关闭注册窗口
}