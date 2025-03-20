#ifndef BACKENDVIEW_H
#define BACKENDVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "recipemanagementview.h"
#include "usermanagementview.h"
#include "feedbackcollectionview.h"

class BackendView : public QWidget {
    Q_OBJECT

public:
    explicit BackendView(int userId, QWidget *parent = nullptr);

private slots:
    void onRecipeManagementClicked(); // 菜谱管理按钮点击事件
    void onUserManagementClicked();   // 用户管理按钮点击事件
    void onFeedbackCollectionClicked(); // 反馈收集按钮点击事件
    void onLogoutClicked(); // 退出登录按钮点击事件

private:
    int userId; // 当前管理员的用户 ID
    QPushButton *recipeManagementButton; // 菜谱管理按钮
    QPushButton *userManagementButton;   // 用户管理按钮
    QPushButton *feedbackCollectionButton; // 反馈收集按钮
    QPushButton *logoutButton; // 退出登录按钮

    RecipeManagementView *recipeManagementView; // 菜谱管理界面
    UserManagementView *userManagementView;     // 用户管理界面
    FeedbackCollectionView *feedbackCollectionView; // 反馈收集界面

    void setupUI(); // 初始化界面
};

#endif // BACKENDVIEW_H