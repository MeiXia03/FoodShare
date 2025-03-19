#include "communityview.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlError>

CommunityView::CommunityView(int recipeId, int userId, QWidget *parent)
    : QWidget(parent), loggedInUserId(userId), currentRecipeId(recipeId) {
    setupUI();
    loadRecipeImageAndTitle(); // 加载食谱图片和标题
    loadComments();           // 加载评论
    loadFeedback();           // 加载用户反馈
}

void CommunityView::setupUI() {
    // 创建食谱标题显示区域
    recipeTitleLabel = new QLabel(this);
    recipeTitleLabel->setAlignment(Qt::AlignCenter);
    recipeTitleLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 10px;");

    // 创建食谱图片显示区域
    recipeImageLabel = new QLabel(this);
    recipeImageLabel->setAlignment(Qt::AlignCenter);
    recipeImageLabel->setStyleSheet("border: 1px solid #ccc; margin-bottom: 10px;");

    // 创建评论列表
    commentsList = new QListWidget(this);
    commentsList->setStyleSheet("border: 1px solid #ccc; padding: 5px; margin-bottom: 10px;");

    // 创建用户反馈表
    feedbackTable = new QTableWidget(this);
    feedbackTable->setColumnCount(3);
    feedbackTable->setHorizontalHeaderLabels({"用户ID", "反馈内容", "状态"});
    feedbackTable->horizontalHeader()->setStretchLastSection(true);
    feedbackTable->setStyleSheet("border: 1px solid #ccc; padding: 5px; margin-bottom: 10px;");

    // 创建评论输入框
    commentInput = new QLineEdit(this);
    commentInput->setPlaceholderText("请输入评论...");
    commentInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");

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
    layout->addWidget(new QLabel("用户反馈:", this));
    layout->addWidget(feedbackTable);
    layout->addWidget(commentInput);
    layout->addWidget(postCommentButton);

    setLayout(layout);
}

void CommunityView::loadRecipeImageAndTitle() {
    // 加载食谱标题和图片
    QSqlQuery query;
    query.prepare("SELECT title FROM recipes WHERE recipe_id = :recipe_id");
    query.bindValue(":recipe_id", currentRecipeId);

    if (!query.exec() || !query.next()) {
        recipeTitleLabel->setText("未找到食谱标题");
        recipeImageLabel->setText("未找到图片");
        return;
    }

    QString title = query.value("title").toString();
    recipeTitleLabel->setText(title);

    QString imagePath = QString(":/res/recipe_%1.jpg").arg(currentRecipeId);
    QPixmap pixmap(imagePath);

    if (pixmap.isNull()) {
        recipeImageLabel->setText("未找到图片");
    } else {
        recipeImageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void CommunityView::loadComments() {
    commentsList->clear();
    QVector<QVariantMap> comments = DatabaseManager::instance().getComments(currentRecipeId);

    for (const auto &comment : comments) {
        QString content = comment["content"].toString();
        QString createdAt = comment["created_at"].toString();
        commentsList->addItem(QString("[%1] %2").arg(createdAt, content));
    }
}

void CommunityView::loadFeedback() {
    feedbackTable->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT user_id, content, status FROM feedback WHERE user_id = :user_id");
    query.bindValue(":user_id", loggedInUserId);

    if (!query.exec()) {
        qDebug() << "加载反馈失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        feedbackTable->insertRow(row);

        feedbackTable->setItem(row, 0, new QTableWidgetItem(query.value("user_id").toString()));
        feedbackTable->setItem(row, 1, new QTableWidgetItem(query.value("content").toString()));
        feedbackTable->setItem(row, 2, new QTableWidgetItem(query.value("status").toString()));

        row++;
    }
}

void CommunityView::onPostCommentClicked() {
    QString content = commentInput->text();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "评论内容不能为空！");
        return;
    }

    if (DatabaseManager::instance().addComment(loggedInUserId, currentRecipeId, content)) {
        QMessageBox::information(this, "成功", "评论发布成功！");
        commentInput->clear();
        loadComments(); // 重新加载评论
    } else {
        QMessageBox::critical(this, "错误", "评论发布失败，请稍后重试！");
    }
}