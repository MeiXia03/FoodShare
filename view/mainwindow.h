#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStackedWidget>
#include "ImageCarousel.h"
#include "CategoryView.h"
#include "SearchView.h"
#include "UploadRecipeView.h"
#include "PersonalInfoView.h" // 添加个人信息界面头文件
#include "ChatRoomView.h" // 添加聊天室功能头文件

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow(int userId, QWidget *parent = nullptr); // 声明构造函数

private slots:
    void onNavigateToHome(); // 切换到主界面
    void onNavigateToCategories(); // 切换到分类功能界面
    void onNavigateToUpload(); // 切换到上传功能界面
    void onNavigateToSearch(); // 切换到搜索功能界面
    void onNavigateToPersonalInfo(); // 切换到个人信息界面
    void onNavigateToChatRoom(); // 切换到聊天室界面

    void switchToImageCarousel(); // 添加方法声明
    void switchToCategoryView();  // 添加方法声明

private:
    QPushButton *homeButton;
    QPushButton *categoriesButton;
    QPushButton *uploadButton;
    QPushButton *searchButton;
    QPushButton *personalInfoButton; // 个人信息按钮
    QPushButton *chatRoomButton; // 聊天室按钮

    QStackedWidget *mainStack; // 主界面内容切换容器
    ImageCarousel *imageCarousel; // 图片轮播组件
    CategoryView *categoryView;
    UploadRecipeView *uploadRecipeView;
    SearchView *searchView;
    PersonalInfoView *personalInfoView; // 个人信息界面
    ChatRoomView *chatRoomView; // 聊天室界面

    int userId; // 当前登录用户的 ID

    void setupTopNavigationBar(); // 设置顶部导航栏
    void setupMainContent(); // 设置主界面内容
};

#endif // MAINWINDOW_H
