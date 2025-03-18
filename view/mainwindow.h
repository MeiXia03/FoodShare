#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStackedWidget>
#include "ImageCarousel.h" // 引入轮播图片类
#include "CategoryView.h" // 引入分类功能界面类
#include "SearchView.h" // 引入搜索功能界面类



class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
void onNavigateToHome(); // 切换到主界面
void onNavigateToCategories(); // 切换到分类功能界面
void onNavigateToUpload(); // 切换到上传功能界面
void onNavigateToCommunity(); // 切换到社区功能界面
void onNavigateToSearch(); // 切换到搜索功能界面

private:
    QPushButton *homeButton;
    QPushButton *categoriesButton;
    QPushButton *uploadButton;
    QPushButton *communityButton;
    QLineEdit *searchBox;
    QPushButton *searchButton;
    QLabel *userInfoLabel;
    QLabel *userAvatar;

    QStackedWidget *mainStack; // 主界面内容切换容器
    ImageCarousel *imageCarousel; // 图片轮播组件
    CategoryView *categoryView; // 分类功能界面
    QWidget *uploadView; // 上传功能界面
    QWidget *communityView; // 社区功能界面
    SearchView *searchView; // 搜索功能界面
    
    void setupTopNavigationBar();
    void setupMainContent();
};

#endif // MAINWINDOW_H
