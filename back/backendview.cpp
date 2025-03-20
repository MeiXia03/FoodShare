#include "backendview.h"
#include "../view/loginview.h" // 引入登录界面
#include <QStackedWidget>


BackendView::BackendView(int userId, QWidget *parent)
    : QWidget(parent), userId(userId) {
    setWindowTitle("后台管理系统");
    resize(800, 600);

    setupUI();
}

void BackendView::setupUI() {
    // 设置背景颜色
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#E0F7FA")); // 浅天蓝色背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 创建左侧按钮
    recipeManagementButton = new QPushButton("菜谱管理", this);
    recipeManagementButton->setStyleSheet("padding: 10px; background-color: #03A9F4; color: white; border: none; border-radius: 5px; font-size: 16px;");
    connect(recipeManagementButton, &QPushButton::clicked, this, &BackendView::onRecipeManagementClicked);

    userManagementButton = new QPushButton("用户管理", this);
    userManagementButton->setStyleSheet("padding: 10px; background-color: #03A9F4; color: white; border: none; border-radius: 5px; font-size: 16px;");
    connect(userManagementButton, &QPushButton::clicked, this, &BackendView::onUserManagementClicked);

    feedbackCollectionButton = new QPushButton("反馈收集", this);
    feedbackCollectionButton->setStyleSheet("padding: 10px; background-color: #03A9F4; color: white; border: none; border-radius: 5px; font-size: 16px;");
    connect(feedbackCollectionButton, &QPushButton::clicked, this, &BackendView::onFeedbackCollectionClicked);

    // 创建退出登录按钮
    logoutButton = new QPushButton("退出登录", this);
    logoutButton->setStyleSheet("padding: 10px; background-color: #f44336; color: white; border: none; border-radius: 5px; font-size: 16px;");
    connect(logoutButton, &QPushButton::clicked, this, &BackendView::onLogoutClicked);

    // 创建左侧布局
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(recipeManagementButton);
    leftLayout->addWidget(userManagementButton);
    leftLayout->addWidget(feedbackCollectionButton);
    leftLayout->addStretch(); // 添加弹性空间
    leftLayout->addWidget(logoutButton); // 添加退出登录按钮

    // 创建右侧内容切换容器
    QStackedWidget *contentStack = new QStackedWidget(this);
    recipeManagementView = new RecipeManagementView(this);
    userManagementView = new UserManagementView(userId, this);
    feedbackCollectionView = new FeedbackCollectionView(this);

    contentStack->addWidget(recipeManagementView);
    contentStack->addWidget(userManagementView);
    contentStack->addWidget(feedbackCollectionView);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout, 1); // 左侧按钮占 1 份空间
    mainLayout->addWidget(contentStack, 3); // 右侧内容占 3 份空间

    setLayout(mainLayout);

    // 默认显示菜谱管理界面
    contentStack->setCurrentWidget(recipeManagementView);
}

void BackendView::onRecipeManagementClicked() {
    QStackedWidget *contentStack = findChild<QStackedWidget *>();
    if (contentStack) {
        contentStack->setCurrentWidget(recipeManagementView); // 切换到菜谱管理界面
    }
}

void BackendView::onUserManagementClicked() {
    QStackedWidget *contentStack = findChild<QStackedWidget *>();
    if (contentStack) {
        contentStack->setCurrentWidget(userManagementView); // 切换到用户管理界面
    }
}

void BackendView::onFeedbackCollectionClicked() {
    QStackedWidget *contentStack = findChild<QStackedWidget *>();
    if (contentStack) {
        contentStack->setCurrentWidget(feedbackCollectionView); // 切换到反馈收集界面
    }
}

void BackendView::onLogoutClicked() {
    // 显示登录界面
    LoginView *loginView = new LoginView();
    loginView->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动释放内存
    loginView->show();

    // 关闭当前窗口
    this->close();
}
