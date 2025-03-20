#include "communityview.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

CommunityView::CommunityView(int recipeId, int userId, QWidget *parent)
    : QWidget(parent), loggedInUserId(userId), currentRecipeId(recipeId) {
    setupUI();
    loadRecipeImageAndTitle(); // 加载食谱图片和标题
    loadComments();           // 加载评论
}

void CommunityView::setupUI() {
    // 创建食谱标题显示区域
    recipeTitleLabel = new QLabel(this);
    recipeTitleLabel->setAlignment(Qt::AlignCenter);
    recipeTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 15px;");

    // 创建食谱图片显示区域
    recipeImageLabel = new QLabel(this);
    recipeImageLabel->setAlignment(Qt::AlignCenter);
    recipeImageLabel->setStyleSheet("border: 2px solid #ccc; margin-bottom: 15px;");

    // 创建评论列表
    commentsList = new QListWidget(this);
    commentsList->setStyleSheet("border: 1px solid #ccc; padding: 10px; margin-bottom: 15px; font-size: 14px;");

    // 创建评论输入框
    commentInput = new QLineEdit(this);
    commentInput->setPlaceholderText("请输入评论...");
    commentInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px; margin-bottom: 10px;");

    // 创建发布评论按钮
    postCommentButton = new QPushButton("发布评论", this);
    postCommentButton->setStyleSheet("padding: 10px; background-color: #4CAF50; color: white; border: none; border-radius: 5px;");
    connect(postCommentButton, &QPushButton::clicked, this, &CommunityView::onPostCommentClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(recipeTitleLabel);
    layout->addWidget(recipeImageLabel);
    layout->addWidget(new QLabel("评论列表:", this));
    layout->addWidget(commentsList);
    layout->addWidget(commentInput);
    layout->addWidget(postCommentButton);

    setLayout(layout);
}

void CommunityView::loadRecipeImageAndTitle() {
    // 加载食谱标题和图片
    QSqlQuery query;
    query.prepare("SELECT title, video_path FROM recipes WHERE recipe_id = :recipe_id");
    query.bindValue(":recipe_id", currentRecipeId);

    if (!query.exec() || !query.next()) {
        recipeTitleLabel->setText("未找到食谱标题");
        recipeImageLabel->setText("未找到图片");
        return;
    }

    QString title = query.value("title").toString();
    QString imagePath = query.value("video_path").toString(); // 从数据库获取图片路径

    recipeTitleLabel->setText(title);

    QPixmap pixmap(imagePath); // 使用文件系统中的路径加载图片

    if (pixmap.isNull()) {
        recipeImageLabel->setText("未找到图片");
    } else {
        recipeImageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void CommunityView::loadComments() {
    commentsList->clear(); // 清空评论列表

    QSqlQuery query;
    query.prepare("SELECT users.username, comments.content, comments.created_at "
                  "FROM comments "
                  "JOIN users ON comments.user_id = users.user_id "
                  "WHERE comments.recipe_id = :recipe_id "
                  "ORDER BY comments.created_at DESC");
    query.bindValue(":recipe_id", currentRecipeId);

    if (!query.exec()) {
        qDebug() << "加载评论失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString username = query.value("username").toString();
        QString content = query.value("content").toString();
        QString createdAt = query.value("created_at").toString();
        commentsList->addItem(QString("[%1] %2: %3").arg(createdAt, username, content));
    }
}

void CommunityView::onPostCommentClicked() {
    QString content = commentInput->text();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "评论内容不能为空！");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO comments (user_id, recipe_id, content) VALUES (:user_id, :recipe_id, :content)");
    query.bindValue(":user_id", loggedInUserId);
    query.bindValue(":recipe_id", currentRecipeId);
    query.bindValue(":content", content);

    if (!query.exec()) {
        qDebug() << "评论发布失败:" << query.lastError().text();
        QMessageBox::critical(this, "错误", "评论发布失败，请稍后重试！");
        return;
    }

    QMessageBox::information(this, "成功", "评论发布成功！");
    commentInput->clear();
    loadComments(); // 重新加载评论
}