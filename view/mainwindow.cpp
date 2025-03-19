#include "mainwindow.h"
#include "uploadrecipeview.h" // 添加头文件
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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
    topNavBar->setStyleSheet("background-color: #ff5722;"); // 设置橙色背景

    // 主界面按钮
    homeButton = new QPushButton("主界面", this);
    homeButton->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    connect(homeButton, &QPushButton::clicked, this, &MainWindow::onNavigateToHome);

    // 分类功能按钮
    categoriesButton = new QPushButton("分类功能", this);
    categoriesButton->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    connect(categoriesButton, &QPushButton::clicked, this, &MainWindow::onNavigateToCategories);

    // 上传功能按钮
    uploadButton = new QPushButton("上传功能", this);
    uploadButton->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::onNavigateToUpload);

    // 搜索功能按钮
    searchButton = new QPushButton("搜索功能", this);
    searchButton->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onNavigateToSearch);

    // 用户头像
    userAvatar = new QLabel(this);
    QPixmap avatarPixmap(":/avatar.jpg");
    userAvatar->setPixmap(avatarPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    userAvatar->setStyleSheet("border: 2px solid white; border-radius: 20px;");

    // 用户信息标签
    userInfoLabel = new QLabel("个人信息", this);
    userInfoLabel->setStyleSheet("color: white; font-size: 16px;");

    // 布局
    QHBoxLayout *leftLayout = new QHBoxLayout();
    leftLayout->addWidget(homeButton);
    leftLayout->addWidget(categoriesButton);
    leftLayout->addWidget(uploadButton);
    leftLayout->addWidget(searchButton);

    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->addWidget(userAvatar);
    rightLayout->addWidget(userInfoLabel);

    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
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
    uploadRecipeView = new UploadRecipeView(this); // 使用 UploadRecipeView
    mainStack->addWidget(uploadRecipeView);

    // 创建搜索功能界面
    searchView = new SearchView(this);
    mainStack->addWidget(searchView);

    // 将主内容切换容器添加到布局
    centralWidget()->layout()->addWidget(mainStack);
}

void MainWindow::onNavigateToHome() {
    mainStack->setCurrentWidget(imageCarousel); // 切换到图片轮播界面
    qDebug() << "切换到主界面";
}

void MainWindow::onNavigateToCategories() {
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
