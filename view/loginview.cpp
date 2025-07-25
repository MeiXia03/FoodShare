#include "loginview.h"
#include "../back/backendview.h"
#include <QMessageBox>

LoginView::LoginView(QWidget *parent) : QWidget(parent) {
    if (!DatabaseManager::instance().connect()) {
        qDebug() << "数据库连接失败";
        return;
    }
    setupUI();
    setWindowTitle("美食分享平台登录"); // 设置窗口标题
    resize(400, 300); // 设置窗口默认大小
}

void LoginView::setupUI() {
    // 创建标题标签
    QLabel *titleLabel = new QLabel("美食分享平台登录", this);
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

    // 创建登录按钮
    loginButton = new QPushButton("登录", this);
    loginButton->setStyleSheet("padding: 10px; background-color: #4CAF50; color: white; border: none; border-radius: 5px;");
    connect(loginButton, &QPushButton::clicked, this, &LoginView::onLoginClicked);

    // 创建注册按钮
    registerButton = new QPushButton("没有账户，立即注册！", this);
    registerButton->setStyleSheet("padding: 10px; background-color: #2196F3; color: white; border: none; border-radius: 5px;");
    connect(registerButton, &QPushButton::clicked, this, &LoginView::onRegisterClicked);

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
    layout->addWidget(loginButton);
    layout->addWidget(registerButton); // 添加注册按钮
    layout->addWidget(statusLabel);

    setLayout(layout);
}

void LoginView::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("用户名或密码不能为空！");
        return;
    }

    // 查询用户信息
    QSqlQuery query;
    query.prepare("SELECT user_id, is_admin FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec() || !query.next()) {
        statusLabel->setText("用户名或密码错误！");
        return;
    }

    int userId = query.value("user_id").toInt(); // 获取用户 ID
    bool isAdmin = query.value("is_admin").toBool();

    // 检查用户是否被封禁
    QSqlQuery blacklistQuery;
    blacklistQuery.prepare("SELECT reason FROM blacklist WHERE user_id = :user_id");
    blacklistQuery.bindValue(":user_id", userId);

    if (blacklistQuery.exec() && blacklistQuery.next()) {
        QString reason = blacklistQuery.value("reason").toString();

        // 弹出封禁原因窗口
        QMessageBox::critical(this, "登录失败", QString("您的账户已被封禁。\n原因：%1").arg(reason));
        return; // 禁止登录
    }

    // 根据管理员标识跳转到不同界面
    if (isAdmin) {
        BackendView *backendView = new BackendView(userId); // 将 userId 传递给 BackendView
        backendView->show();
    } else {
        MainWindow *mainWindow = new MainWindow(userId); // 将 userId 传递给 MainWindow
        mainWindow->show();
    }

    // 关闭登录界面
    this->close();
}

void LoginView::onRegisterClicked() {
    // 打开模态的注册窗口
    RegisterView *registerView = new RegisterView();
    registerView->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动释放内存
    registerView->show();
}
