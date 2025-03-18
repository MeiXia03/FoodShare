#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onNavigateToHome();
    void onNavigateToCategories();
    void onNavigateToUpload();
    void onNavigateToCommunity();
    void onSearchClicked();
    void switchImage(); // 切换图片的槽函数

private:
    QPushButton *homeButton;
    QPushButton *categoriesButton;
    QPushButton *uploadButton;
    QPushButton *communityButton;
    QLineEdit *searchBox;
    QPushButton *searchButton;
    QLabel *userInfoLabel;
    QLabel *userAvatar;

    QStackedWidget *imageStack; // 用于图片轮播的 QStackedWidget
    QTimer *imageSwitchTimer;   // 定时器，用于切换图片

    void setupTopNavigationBar();
    void setupMainContent(); // 设置主界面内容
};

#endif // MAINWINDOW_H