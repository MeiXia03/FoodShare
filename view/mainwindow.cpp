#include "mainwindow.h"
#include <QPixmap>
#include <QSpacerItem>
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

    // 社区功能按钮
    communityButton = new QPushButton("社区功能", this);
    communityButton->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    connect(communityButton, &QPushButton::clicked, this, &MainWindow::onNavigateToCommunity);

    // 搜索框
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("搜索食谱或技巧...");
    searchBox->setStyleSheet("padding: 5px; border-radius: 5px; border: none;");

    // 搜索按钮
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("color: #ff5722; background-color: white; font-size: 14px; padding: 5px 10px; border-radius: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    // 用户头像
    userAvatar = new QLabel(this);
    QPixmap avatarPixmap(":/resources/avatar.png"); // 替换为实际头像路径
    userAvatar->setPixmap(avatarPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    userAvatar->setStyleSheet("border: 2px solid white; border-radius: 20px;");

    // 用户信息标签
    userInfoLabel = new QLabel("个人信息", this);
    userInfoLabel->setStyleSheet("color: white; font-size: 16px;");

    // 添加左侧导航按钮
    QHBoxLayout *leftLayout = new QHBoxLayout();
    leftLayout->addWidget(homeButton);
    leftLayout->addWidget(categoriesButton);
    leftLayout->addWidget(uploadButton);
    leftLayout->addWidget(communityButton);

    // 添加搜索框
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);

    // 添加右侧用户信息
    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->addWidget(userAvatar);
    rightLayout->addWidget(userInfoLabel);

    // 添加到主布局
    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(searchLayout);
    mainLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(rightLayout);

    // 设置导航栏为窗口顶部
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(topNavBar);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMainContent() {
    // 创建图片轮播容器
    imageStack = new QStackedWidget(this);

    // 添加图片到 QStackedWidget
    QStringList imagePaths = {
        ":/image1.jpg",
        ":/image2.jpg",
        ":/image3.jpg",
        ":/image4.jpg"
    };

    for (const QString &path : imagePaths) {
        QLabel *imageLabel = new QLabel(this);
        QPixmap pixmap(path);
        imageLabel->setPixmap(pixmap.scaled(800, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageStack->addWidget(imageLabel);
    }

    // 创建定时器切换图片
    imageSwitchTimer = new QTimer(this);
    connect(imageSwitchTimer, &QTimer::timeout, this, &MainWindow::switchImage);
    imageSwitchTimer->start(3000); // 每 3 秒切换一次图片

    // 将图片轮播添加到主布局
    centralWidget()->layout()->addWidget(imageStack);
}

void MainWindow::switchImage() {
    int currentIndex = imageStack->currentIndex();
    int nextIndex = (currentIndex + 1) % imageStack->count();
    imageStack->setCurrentIndex(nextIndex);
}

void MainWindow::onNavigateToHome() {
    qDebug() << "导航到主界面";
}

void MainWindow::onNavigateToCategories() {
    qDebug() << "导航到分类功能";
}

void MainWindow::onNavigateToUpload() {
    qDebug() << "导航到上传功能";
}

void MainWindow::onNavigateToCommunity() {
    qDebug() << "导航到社区功能";
}

void MainWindow::onSearchClicked() {
    qDebug() << "搜索内容:" << searchBox->text();
}
