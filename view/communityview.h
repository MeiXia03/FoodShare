#ifndef COMMUNITYVIEW_H
#define COMMUNITYVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include <QPixmap>
#include "../sql/DatabaseManager.h"

class CommunityView : public QWidget {
    Q_OBJECT

public:
    explicit CommunityView(int recipeId, int userId, QWidget *parent = nullptr); // 接收食谱ID和用户ID

private slots:
    void onPostCommentClicked(); // 发布评论按钮点击事件

private:
    int loggedInUserId; // 当前登录用户ID
    int currentRecipeId; // 当前食谱ID
    QLabel *recipeImageLabel; // 显示食谱图片
    QLabel *recipeTitleLabel; // 显示食谱标题
    QListWidget *commentsList; // 显示评论列表
    QTableWidget *feedbackTable; // 显示用户反馈表
    QLineEdit *commentInput; // 评论输入框
    QPushButton *postCommentButton; // 发布评论按钮

    void setupUI(); // 初始化界面
    void loadRecipeImageAndTitle(); // 加载食谱图片和标题
    void loadComments(); // 加载评论
    void loadFeedback(); // 加载用户反馈
};

#endif // COMMUNITYVIEW_H