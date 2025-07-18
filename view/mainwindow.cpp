#include "mainwindow.h"
#include "uploadrecipeview.h" 
#include <QVBoxLayout>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(int userId, QWidget *parent) 
    : QMainWindow(parent), userId(userId) { // 接收 userId 参数
    setWindowTitle("美食分享系统");
    resize(800, 600);
    // 设置顶部导航栏
    setupTopNavigationBar();

    // 设置主界面内容
    setupMainContent();
}

void MainWindow::setupTopNavigationBar() {
    // 创建导航栏容器
    QWidget *topNavBar = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(topNavBar);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(20);
    topNavBar->setStyleSheet("background-color: #ff5722; padding: 10px;"); // 设置橙色背景并增加内边距

    // 主界面按钮
    homeButton = new QPushButton("主界面", this);
    homeButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #4CAF50; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(homeButton, &QPushButton::clicked, this, &MainWindow::onNavigateToHome);

    // 分类功能按钮
    categoriesButton = new QPushButton("分类功能", this);
    categoriesButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #2196F3; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(categoriesButton, &QPushButton::clicked, this, &MainWindow::onNavigateToCategories);

    // 上传功能按钮
    uploadButton = new QPushButton("上传功能", this);
    uploadButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #FF9800; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::onNavigateToUpload);

    // 搜索功能按钮
    searchButton = new QPushButton("搜索功能", this);
    searchButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #9C27B0; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onNavigateToSearch);

    // 聊天室按钮
    chatRoomButton = new QPushButton("聊天室", this);
    chatRoomButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #607D8B; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(chatRoomButton, &QPushButton::clicked, this, &MainWindow::onNavigateToChatRoom);

    // 个人信息按钮
    personalInfoButton = new QPushButton("个人信息", this);
    personalInfoButton->setStyleSheet(
        "color: white; font-size: 16px; font-weight: bold; background-color: #607D8B; "
        "border: none; border-radius: 5px; padding: 10px 20px; margin-right: 10px;");
    connect(personalInfoButton, &QPushButton::clicked, this, &MainWindow::onNavigateToPersonalInfo);

    // 布局
    QHBoxLayout *leftLayout = new QHBoxLayout();
    leftLayout->addWidget(homeButton);
    leftLayout->addWidget(categoriesButton);
    leftLayout->addWidget(uploadButton);
    leftLayout->addWidget(searchButton);
    leftLayout->addWidget(chatRoomButton);

    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->addWidget(personalInfoButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(rightLayout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(topNavBar);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMainContent() {
    // 创建主内容切换容器
    mainStack = new QStackedWidget(this);

    // 创建图片轮播组件
    imageCarousel = new ImageCarousel(this);
    mainStack->addWidget(imageCarousel);

    // 创建分类功能界面
    categoryView = new CategoryView(this);
    mainStack->addWidget(categoryView);

    // 创建上传功能界面
    uploadRecipeView = new UploadRecipeView(userId, this); // 传递 userId
    mainStack->addWidget(uploadRecipeView);

    // 创建搜索功能界面
    searchView = new SearchView(userId, this); // 传递 userId
    mainStack->addWidget(searchView);

    // 创建聊天室界面
    chatRoomView = new ChatRoomView(userId, this); // 传递 userId
    mainStack->addWidget(chatRoomView);

    // 创建个人信息界面
    personalInfoView = new PersonalInfoView(userId, this); // 传递用户 ID
    mainStack->addWidget(personalInfoView);

    // 将主内容切换容器添加到布局
    centralWidget()->layout()->addWidget(mainStack);
}

void MainWindow::onNavigateToHome() {
    imageCarousel->refreshData(); // 刷新图片轮播数据
    mainStack->setCurrentWidget(imageCarousel); // 切换到图片轮播界面
    qDebug() << "切换到主界面";
}

void MainWindow::onNavigateToCategories() {
    categoryView->refreshData(); // 刷新分类功能数据
    mainStack->setCurrentWidget(categoryView); // 切换到分类功能界面
    qDebug() << "切换到分类功能界面";
}

void MainWindow::onNavigateToUpload() {
    mainStack->setCurrentWidget(uploadRecipeView); // 切换到上传功能界面
    qDebug() << "切换到上传功能界面";
}

void MainWindow::onNavigateToSearch() {
    mainStack->setCurrentWidget(searchView); // 切换到搜索功能界面
    qDebug() << "切换到搜索功能界面";
}

void MainWindow::onNavigateToPersonalInfo() {
    mainStack->setCurrentWidget(personalInfoView); // 切换到个人信息界面
    qDebug() << "切换到个人信息界面";
}

void MainWindow::onNavigateToChatRoom() {
    mainStack->setCurrentWidget(chatRoomView); // 切换到聊天室界面
    qDebug() << "切换到聊天室界面，用户ID:" << userId;
}

void MainWindow::switchToImageCarousel() {
    imageCarousel->refreshData(); // 刷新图片轮播数据
    mainStack->setCurrentWidget(imageCarousel);
}

void MainWindow::switchToCategoryView() {
    categoryView->refreshData(); // 刷新分类视图数据
    mainStack->setCurrentWidget(categoryView);
}